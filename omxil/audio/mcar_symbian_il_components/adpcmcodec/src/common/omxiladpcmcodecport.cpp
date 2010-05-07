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
#include "log.h"
#include "omxilutil.h"
#include "omxiladpcmcodecport.h"


_LIT8(KMimeTypeAudioImaAdpcm, "audio/ima.adpcm"); // There is no valid mime type

COmxILAdPcmCodecPort* COmxILAdPcmCodecPort::NewL(
			const TOmxILCommonPortData& aCommonPortData,
			const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats,
			const OMX_AUDIO_PARAM_ADPCMTYPE& aPcmModeType)
	{
	DEBUG_PRINTF(_L8("COmxILAdPcmCodecPort::NewL"));

	COmxILAdPcmCodecPort* self =
		new(ELeave)COmxILAdPcmCodecPort(aCommonPortData, aPcmModeType);

	CleanupStack::PushL(self);
	self->ConstructL(aSupportedAudioFormats);
	CleanupStack::Pop();

	return self;
	}


COmxILAdPcmCodecPort::COmxILAdPcmCodecPort(const TOmxILCommonPortData& aCommonPortData,
		 const OMX_AUDIO_PARAM_ADPCMTYPE& aPcmModeType)
	:	COmxILAudioPort(aCommonPortData),
		iParamAudioAdPcm(aPcmModeType)
	{
	DEBUG_PRINTF(_L8("COmxILAdPcmCodecPort::NewL"));	
	}


void COmxILAdPcmCodecPort::ConstructL(const RArray<OMX_AUDIO_CODINGTYPE>& aSupportedAudioFormats)
	{
	DEBUG_PRINTF(_L8("COmxILAdPcmCodecPort::ConstructL"));
	
	// Debug checks (release version will leave in ConstructL instead)
	ASSERT(aSupportedAudioFormats.Count() == 1);
	ASSERT(aSupportedAudioFormats[0] == OMX_AUDIO_CodingADPCM);

	if (aSupportedAudioFormats.Count() != 1 || aSupportedAudioFormats[0] != OMX_AUDIO_CodingADPCM)
		{
		User::Leave(KErrNotSupported);
		}

	COmxILAudioPort::ConstructL(aSupportedAudioFormats);

	// before constructL comlpetes, we must be finished with the iParamPortDefinition
	iParamPortDefinition.eDomain = OMX_PortDomainAudio;

	TUint8* mimeptr = const_cast<TUint8*>(KMimeTypeAudioImaAdpcm().Ptr());	// KMimeTypeAudioPcm is already nul-terminated since it's made from a literal C-style string
							
	iParamPortDefinition.format.audio.cMIMEType = reinterpret_cast<OMX_STRING>(mimeptr);

	iParamPortDefinition.format.audio.bFlagErrorConcealment = OMX_FALSE;
	iParamPortDefinition.format.audio.eEncoding = OMX_AUDIO_CodingADPCM;
	}


COmxILAdPcmCodecPort::~COmxILAdPcmCodecPort()
	{
	DEBUG_PRINTF(_L8("COmxILAdPcmCodecPort::~COmxILAdPcmDecoderAPB0Port"));
	// nothing to do
	}

OMX_ERRORTYPE COmxILAdPcmCodecPort::GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const
	{
    DEBUG_PRINTF(_L8("COmxILAdPcmCodecPort::GetLocalOmxParamIndexes"));

	// Always collect param indexes from parent
	OMX_ERRORTYPE omxRetValue = COmxILAudioPort::GetLocalOmxParamIndexes(aIndexArray);
	if (OMX_ErrorNone != omxRetValue)
		{
		return omxRetValue;
		}

	TInt err = aIndexArray.InsertInOrder(OMX_IndexParamAudioAdpcm);
	if (KErrNone != err && KErrAlreadyExists != err)	// Note that index duplication is OK.
		{
		return OMX_ErrorInsufficientResources;
		}

	return OMX_ErrorNone;
	}

OMX_ERRORTYPE COmxILAdPcmCodecPort::GetLocalOmxConfigIndexes(RArray<TUint>& aIndexArray) const
	{
    DEBUG_PRINTF(_L8("COmxILAdPcmDecoderAPB0Port::GetLocalOmxConfigIndexes"));
    return COmxILAudioPort::GetLocalOmxConfigIndexes(aIndexArray);
	}

OMX_ERRORTYPE COmxILAdPcmCodecPort::GetParameter(OMX_INDEXTYPE aParamIndex, TAny* apComponentParameterStructure) const
	{
    DEBUG_PRINTF(_L8("COmxILAdPcmCodecPort::GetParameter"));

    // we only have OMX_IndexParamAudioAdpcm - pass all other requests to our parent
    if (aParamIndex!=OMX_IndexParamAudioAdpcm)
		{
		return COmxILAudioPort::GetParameter(aParamIndex, apComponentParameterStructure);
		}

    OMX_ERRORTYPE err = TOmxILUtil::CheckOmxStructSizeAndVersion( apComponentParameterStructure, sizeof iParamAudioAdPcm );
    if (err==OMX_ErrorNone)
    	{
    	// export our PCM mode settings
    	OMX_AUDIO_PARAM_ADPCMTYPE* const pcmmode = static_cast<OMX_AUDIO_PARAM_ADPCMTYPE*>(apComponentParameterStructure);
    	*pcmmode = iParamAudioAdPcm;
    	}

	return err;
	}


OMX_ERRORTYPE COmxILAdPcmCodecPort::SetParameter(OMX_INDEXTYPE aParamIndex, const TAny* apComponentParameterStructure, TBool& aUpdateProcessingFunction)
	{
    DEBUG_PRINTF(_L8("COmxILAdPcmCodecPort::SetParameter"));

    // we only have OMX_IndexParamAudioAdpcm - pass all other requests to our parent
    if (aParamIndex!=OMX_IndexParamAudioAdpcm)
		{
		return COmxILAudioPort::SetParameter(aParamIndex, apComponentParameterStructure, aUpdateProcessingFunction);
		}

    OMX_ERRORTYPE err = TOmxILUtil::CheckOmxStructSizeAndVersion( const_cast<TAny*>(apComponentParameterStructure), sizeof iParamAudioAdPcm );
    if (err!=OMX_ErrorNone)
    	{
    	return err;
    	}

    // get and sanity check the new settings
	const OMX_AUDIO_PARAM_ADPCMTYPE* pcmmode = static_cast<const OMX_AUDIO_PARAM_ADPCMTYPE*>(apComponentParameterStructure);
	if (!CheckPcmModeParamsAreSane(*pcmmode))
		{
		return OMX_ErrorBadParameter;
		}

	// preserve the new settings and update the processing function
	iParamAudioAdPcm = *pcmmode;
	aUpdateProcessingFunction = EFalse;//no need to update processing function as we handle ALL changes for AdPcm here

	return OMX_ErrorNone;
	}


OMX_ERRORTYPE COmxILAdPcmCodecPort::SetFormatInPortDefinition(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition, TBool& aUpdateProcessingFunction)
	{
	DEBUG_PRINTF(_L8("COmxILAdPcmCodecPort::SetFormatInPortDefinition"));

	// Currently not changing cMIMEType type of this port
	// Same thing for pNativeRender and eEncoding
	iParamPortDefinition.format.audio.bFlagErrorConcealment = aPortDefinition.format.audio.bFlagErrorConcealment;

	// Error concealment not currently needed at the processing function
	aUpdateProcessingFunction = EFalse;

	return OMX_ErrorNone;
	}

TBool COmxILAdPcmCodecPort::IsTunnelledPortCompatible( const OMX_PARAM_PORTDEFINITIONTYPE& /*aPortDefinition*/) const
	{
	return ETrue;
	//TODO: Restore proper port compatibility chek
	/*
	DEBUG_PRINTF(_L8("COmxILAdPcmCodecPort::IsTunnelledPortCompatible"));

	if(aPortDefinition.eDomain != iParamPortDefinition.eDomain
		|| aPortDefinition.format.audio.eEncoding != iParamPortDefinition.format.audio.eEncoding)
		{
		return EFalse;
		}

	// check that the MIME types match
	TPtrC8 tunnelMimeType( reinterpret_cast<TUint8* const>(aPortDefinition.format.audio.cMIMEType), User::StringLength(reinterpret_cast<TUint8* const>(aPortDefinition.format.audio.cMIMEType)) );
	TPtrC8 myMimeType( reinterpret_cast<TUint8* const>(iParamPortDefinition.format.audio.cMIMEType), User::StringLength(reinterpret_cast<TUint8* const>(iParamPortDefinition.format.audio.cMIMEType)) );

	if (tunnelMimeType != myMimeType)
		{
		return EFalse;
		}
	
	return ETrue;
	*/
	}


TBool COmxILAdPcmCodecPort::CheckPcmModeParamsAreSane(const OMX_AUDIO_PARAM_ADPCMTYPE& aPcmModeType) const
	{
	return (aPcmModeType.nBitsPerSample == 4)
		&& (aPcmModeType.nPortIndex == iParamAudioAdPcm.nPortIndex)//port number changes are not allowed
		&& (aPcmModeType.nChannels == 1 || aPcmModeType.nChannels == 2)
		;
	}
