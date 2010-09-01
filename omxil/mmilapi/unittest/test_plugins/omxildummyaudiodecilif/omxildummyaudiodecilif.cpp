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

#include "omxildummyaudiodecilif.h" 
#include "iltestuids.hrh"
#include "omxilspecversion.h"
#include "tilstructtypes.h"
#include "tomxilstruct.h"
#include "milcomponentportif.h"
#include <ecom/implementationproxy.h>
#include <a3f/audioformatuids.h>


// _________________________________________________________________________________

const TImplementationProxy ImplementationTable[] = 
    {
    IMPLEMENTATION_PROXY_ENTRY(KUidOmxILDummyAudioDecMILIFPlugin , COmxILDummyAudioDecILIF::NewL)
    };

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

    return ImplementationTable;
    };
// _________________________________________________________________________________


const TUint KMonoChannel     = 1;
const TUint KStereoChannel    = 2;
const TInt  KPcm16BitsPerSample = 16;    


COmxILDummyAudioDecILIF* COmxILDummyAudioDecILIF::NewL()
    {
    COmxILDummyAudioDecILIF* self = new (ELeave) COmxILDummyAudioDecILIF;
    return self;
    }

COmxILDummyAudioDecILIF::COmxILDummyAudioDecILIF()
    { 
    }

COmxILDummyAudioDecILIF::~COmxILDummyAudioDecILIF()
    {
    }



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

Mandatory params across all standard components
===============================================
    OMX_IndexParamPortDefinition    OMX_PARAM_PORTDEFINITIONTYPE    (KUidTPortDefinitionType, TPortDefinitionType)
    OMX_IndexParamAudioInit
    OMX_IndexParamCompBufferSupplier

Mandatory params across all audio standard components
=====================================================
    OMX_IndexParamAudioPortFormat    OMX_AUDIO_PARAM_PORTFORMATTYPE    (KUidTAudioPortFormatType, TAudioPortFormatType)

Component role: audio_decoder.aac
=================================
- APB+0: Accepts encoded audio
    OMX_IndexParamAudioAac            OMX_AUDIO_PARAM_AACPROFILETYPE    (KUidTPortAACProfileType, TPortAACProfileType)
- APB+1: Emits decoded audio
    OMX_IndexParamAudioPcm            OMX_AUDIO_PARAM_PCMMODETYPE       (KUidTPortPCMType, TPortPCMType)

Component role: audio_renderer.pcm
==================================
- APB+0: Accepts audio for rendering:
    (OMX_IndexParamAudioPcm,        OMX_AUDIO_PARAM_PCMMODETYPE)    (KUidTPortPCMType, TPortPCMType)
    (OMX_IndexConfigAudioVolume,    OMX_AUDIO_CONFIG_VOLUMETYPE)    (KUidTPortVolumeType, TPortVolumeType)
    (OMX_IndexConfigAudioMute,      OMX_AUDIO_CONFIG_MUTETYPE)      (KUidTPortMuteType, TPortMuteType)
    
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

TInt COmxILDummyAudioDecILIF::SetConfig(const TILStruct& aConfig, 
                                        const MILComponentPortIf* aComponentPort)
    {

    TInt portIndex = 0xFF; // Initialize to invalid port index
    if (aComponentPort)
	    {
    	portIndex = aComponentPort->PortIndex();
	    }
    else
		{
		// All parameters and configs are targetted to the port.
		return KErrArgument;
		}
		
	TInt error = KErrNone;   
        
    switch (aConfig.Uid().iUid)
        {
        case KUidOMXILStructType:
            {    
            const TOMXILStructParam* omxILStructParam = static_cast<const TOMXILStructParam*> (&aConfig);
            switch (omxILStructParam->Index())
                {
                case OMX_IndexParamPortDefinition:
                    {
                    OMX_PARAM_PORTDEFINITIONTYPE* config = reinterpret_cast<OMX_PARAM_PORTDEFINITIONTYPE*>(
                    									   const_cast<TOMXILStructParam*>(omxILStructParam)->Param());    
            		ASSERT(config);
                    config->nPortIndex = portIndex;
                    config->nSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
                    error = OmxSetParameter(OMX_IndexParamPortDefinition, config);
                    break;
                    }
                default:
                    {
                    error = KErrNotSupported;
                    }
                }
            break;            
            }
            
        case KUidMMFTTaskConfig:
            {
            const TTaskConfig* config = TILTaskConfig::GetStructure(aConfig);    
            ASSERT(config);

            TInt sampleRate = config->iRate;
            TInt channels = (config->iStereoMode & ETaskMono)? KMonoChannel : KStereoChannel;
            TBool interleaved = (config->iStereoMode & ETaskInterleaved)?ETrue : EFalse;

            TUid format;
            error = GetComponentIfRole(format);
            if (error != KErrNone)
                {
                break;
                }
            
            if (format == KUidFormatOGG)
                {
                if (aComponentPort->PortDirection()== EDirOutput)
                    {
                    // Set Output Port (PCM16)
                    OMX_AUDIO_PARAM_PCMMODETYPE pcm;
                    pcm.nVersion = KOMXILSpecVersion;
                    pcm.nSize = sizeof(OMX_AUDIO_PARAM_PCMMODETYPE);
                    pcm.nPortIndex = portIndex;
                    pcm.nSamplingRate = sampleRate;
                    pcm.ePCMMode = OMX_AUDIO_PCMModeLinear;
                    pcm.eNumData = OMX_NumericalDataSigned; 
                    pcm.nBitPerSample = KPcm16BitsPerSample;
                    pcm.nChannels = channels;
                    pcm.bInterleaved = (OMX_BOOL)interleaved;
                    
                    error = OmxSetParameter(OMX_IndexParamAudioPcm, &pcm);
                    }                        
                else if (aComponentPort->PortDirection() == EDirInput)
                    {
                    // Set Input Port (Vorbis)
                    OMX_AUDIO_PARAM_VORBISTYPE vorbis;
                    vorbis.nVersion = KOMXILSpecVersion;
                    vorbis.nSize = sizeof(OMX_AUDIO_PARAM_VORBISTYPE);
                    vorbis.nPortIndex = portIndex;
                    
                    error = OmxSetParameter(OMX_IndexParamAudioVorbis, &vorbis);
                    }
                }
            else if (format == KUidFormatPCM16)
                {
                if (aComponentPort->PortDirection()== EDirInput)
                    {
                    // Set Input Port (PCM16)
                    OMX_AUDIO_PARAM_PCMMODETYPE pcm;
                    pcm.nVersion = KOMXILSpecVersion;
                    pcm.nSize = sizeof(OMX_AUDIO_PARAM_PCMMODETYPE);
                    pcm.nPortIndex = portIndex;
                    pcm.nSamplingRate = sampleRate;
                    pcm.ePCMMode = OMX_AUDIO_PCMModeLinear;
                    pcm.eNumData = OMX_NumericalDataSigned; 
                    pcm.nBitPerSample = KPcm16BitsPerSample;
                    pcm.nChannels = channels;
                    pcm.bInterleaved = (OMX_BOOL)interleaved;
                    
                    error = OmxSetParameter(OMX_IndexParamAudioPcm, &pcm);
                    }
                else
                    {
                    error = KErrNotSupported;    
                    }
                }
            else
                {
                error = KErrNotSupported;
                }
            break;    
            }
        case KUidTPortDefinitionType:
            {
            OMX_PARAM_PORTDEFINITIONTYPE* config = const_cast<OMX_PARAM_PORTDEFINITIONTYPE*>(    
															  TILPortDefinition::GetStructure(aConfig));
            ASSERT(config);
            config->nVersion = KOMXILSpecVersion;
            config->nSize = sizeof(OMX_AUDIO_PARAM_PORTFORMATTYPE);
            config->nPortIndex = portIndex;
            error = OmxSetParameter(OMX_IndexParamPortDefinition, config);
            break;
            }
        case KUidTAudioPortFormatType:
            {
            OMX_AUDIO_PARAM_PORTFORMATTYPE* config = const_cast<OMX_AUDIO_PARAM_PORTFORMATTYPE*>(
            													TILAudioPortFormat::GetStructure(aConfig));    
            ASSERT(config);
	        config->nVersion = KOMXILSpecVersion;
            config->nSize = sizeof(OMX_AUDIO_PARAM_PORTFORMATTYPE);
            config->nPortIndex = portIndex;
            error = OmxSetParameter(OMX_IndexParamAudioPortFormat, config);
            break;
            }
        case KUidTPortPCMModeType:
            {
            OMX_AUDIO_PARAM_PCMMODETYPE* config = const_cast<OMX_AUDIO_PARAM_PCMMODETYPE*>(
            												 TILPortPCMMode::GetStructure(aConfig));    
            ASSERT(config);
            config->nVersion = KOMXILSpecVersion;
            config->nSize = sizeof(OMX_AUDIO_PARAM_PCMMODETYPE);
            config->nPortIndex = portIndex;
            error = OmxSetParameter(OMX_IndexParamAudioPcm, config);
            break;
            }
        case KUidTPortAACProfileType:
            {
            OMX_AUDIO_PARAM_AACPROFILETYPE* config = const_cast<OMX_AUDIO_PARAM_AACPROFILETYPE*>(
            													TILPortAACProfile::GetStructure(aConfig));    
            ASSERT(config);
            config->nVersion = KOMXILSpecVersion;
            config->nSize = sizeof(OMX_AUDIO_PARAM_AACPROFILETYPE);
            config->nPortIndex = portIndex;
            error = OmxSetParameter(OMX_IndexParamAudioAac, config);
            break;
            }
        case KUidTPortVorbisType:
            {
            OMX_AUDIO_PARAM_VORBISTYPE* config = const_cast<OMX_AUDIO_PARAM_VORBISTYPE*>(
            												TILPortVorbis::GetStructure(aConfig));    
            ASSERT(config);
            config->nVersion = KOMXILSpecVersion;
            config->nSize = sizeof(OMX_AUDIO_PARAM_VORBISTYPE);
            config->nPortIndex = portIndex;
            error = OmxSetParameter(OMX_IndexParamAudioVorbis, config);
            break;
            }
        case KUidTPortVolumeType:
            {
            OMX_AUDIO_CONFIG_VOLUMETYPE* config = const_cast<OMX_AUDIO_CONFIG_VOLUMETYPE*>(
            												 TILPortVolume::GetStructure(aConfig));    
            ASSERT(config);
            config->nVersion = KOMXILSpecVersion;
            config->nSize = sizeof(OMX_AUDIO_CONFIG_VOLUMETYPE);
            config->nPortIndex = portIndex;
            error = OmxSetConfig(OMX_IndexConfigAudioVolume, config);
            break;
            }
        case KUidTPortMuteType:
            {
            OMX_AUDIO_CONFIG_MUTETYPE* config = const_cast<OMX_AUDIO_CONFIG_MUTETYPE*>(
            											   TILPortMute::GetStructure(aConfig));    
            ASSERT(config);
            config->nVersion = KOMXILSpecVersion;
            config->nSize = sizeof(OMX_AUDIO_CONFIG_MUTETYPE);
            config->nPortIndex = portIndex;
            error = OmxSetConfig(OMX_IndexConfigAudioMute, config);
            break;
            }
        default:
            {
            error = KErrNotSupported;
            }        
        }
    return error;
    }
    

TInt COmxILDummyAudioDecILIF::GetConfig(TILStruct& aConfig, 
                                        const MILComponentPortIf* aComponentPort) const
    {
    TInt portIndex = 0xFF; // Initialize to invalid port index
    if (aComponentPort)
	    {
    	portIndex = aComponentPort->PortIndex();
	    }
    else
		{
		// All parameters and configs are targetted to the port.
		return KErrArgument;
		}

    TInt error = KErrNone;

    switch (aConfig.Uid().iUid)
        {
        case KUidOMXILStructType:
            {    
            TOMXILStructParam* omxILStructParam = static_cast<TOMXILStructParam*> (&aConfig);
            switch (omxILStructParam->Index())
                {
                case OMX_IndexParamPortDefinition:
                    {
                    OMX_PARAM_PORTDEFINITIONTYPE* config = reinterpret_cast<OMX_PARAM_PORTDEFINITIONTYPE*>(omxILStructParam->Param());    
		            ASSERT(config);
                    config->nVersion = KOMXILSpecVersion;
                    config->nSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
                    config->nPortIndex = portIndex;
                    error = OmxGetParameter(OMX_IndexParamPortDefinition, config);
                    break;
                    }
                default:
                    {
                    error = KErrNotSupported;
                    }
                }
            break;            
            }

        case KUidTPortDefinitionType:
            {
            OMX_PARAM_PORTDEFINITIONTYPE* config = TILPortDefinition::GetStructure(aConfig);    
            ASSERT(config);
            config->nVersion = KOMXILSpecVersion;
            config->nSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
            config->nPortIndex = portIndex;
            error = OmxGetParameter(OMX_IndexParamPortDefinition, config);
            break;
            }
        case KUidTAudioPortFormatType:
            {
            OMX_AUDIO_PARAM_PORTFORMATTYPE* config = TILAudioPortFormat::GetStructure(aConfig);    
            ASSERT(config);
            config->nVersion = KOMXILSpecVersion;
            config->nSize = sizeof(OMX_AUDIO_PARAM_PORTFORMATTYPE);
            config->nPortIndex = portIndex;
            error = OmxGetParameter(OMX_IndexParamAudioPortFormat, config);
            break;
            }
        case KUidTPortPCMModeType:
            {
            OMX_AUDIO_PARAM_PCMMODETYPE* config = TILPortPCMMode::GetStructure(aConfig);    
            ASSERT(config);
            config->nVersion = KOMXILSpecVersion;
            config->nSize = sizeof(OMX_AUDIO_PARAM_PCMMODETYPE);
            config->nPortIndex = portIndex;
            error = OmxGetParameter(OMX_IndexParamAudioPcm, config);
            break;
            }
        case KUidTPortAACProfileType:
            {
            OMX_AUDIO_PARAM_AACPROFILETYPE* config = TILPortAACProfile::GetStructure(aConfig);    
            ASSERT(config);
            config->nVersion = KOMXILSpecVersion;
            config->nSize = sizeof(OMX_AUDIO_PARAM_AACPROFILETYPE);
            config->nPortIndex = portIndex;
            error = OmxGetParameter(OMX_IndexParamAudioAac, config);
            break;
            }
        case KUidTPortVorbisType:
            {
            OMX_AUDIO_PARAM_VORBISTYPE* config = TILPortVorbis::GetStructure(aConfig);    
            ASSERT(config);
            config->nVersion = KOMXILSpecVersion;
            config->nSize = sizeof(OMX_AUDIO_PARAM_VORBISTYPE);
            config->nPortIndex = portIndex;
            error = OmxGetParameter(OMX_IndexParamAudioVorbis, config);
            break;
            }
        case KUidTPortVolumeType:
            {
            OMX_AUDIO_CONFIG_VOLUMETYPE* config = TILPortVolume::GetStructure(aConfig);    
            ASSERT(config);
            config->nVersion = KOMXILSpecVersion;
            config->nSize = sizeof(OMX_AUDIO_CONFIG_VOLUMETYPE);
            config->nPortIndex = portIndex;
            error = OmxGetConfig(OMX_IndexConfigAudioVolume, config);
            break;
            }
        case KUidTPortMuteType:
            {
            OMX_AUDIO_CONFIG_MUTETYPE* config = TILPortMute::GetStructure(aConfig);    
            ASSERT(config);
            config->nVersion = KOMXILSpecVersion;
            config->nSize = sizeof(OMX_AUDIO_CONFIG_MUTETYPE);
            config->nPortIndex = portIndex;
            error = OmxGetConfig(OMX_IndexConfigAudioMute, config);
            break;
            }
        default:
            {
            error = KErrNotSupported;
            }
        }

    return error;    
    }
    
TInt COmxILDummyAudioDecILIF::SetExtConfig(const TDesC8& aParameterName, 
                                           const TILStruct& aConfig, 
                                           const MILComponentPortIf* aComponentPort)
    {
    TInt portIndex = 0xFF; // Initialize to invalid port index
    if (aComponentPort)
	    {
    	portIndex = aComponentPort->PortIndex();
	    }
    else
		{
		// All parameters and configs are targetted to the port.
		return KErrArgument;
		}

    OMX_INDEXTYPE indexType;
    TInt error = KErrNone;
    error = OmxGetExtensionIndex(aParameterName, &indexType);

    if (error == KErrNone)
        {
        switch (aConfig.Uid().iUid)
            {
            case KUidTPortDefinitionType:
                {
                OMX_PARAM_PORTDEFINITIONTYPE* config = const_cast<OMX_PARAM_PORTDEFINITIONTYPE*>(TILPortDefinition::GetStructure(aConfig));    
	            ASSERT(config);
                config->nVersion = KOMXILSpecVersion;
                config->nSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
                config->nPortIndex = portIndex;
                error = OmxSetParameter(OMX_IndexParamPortDefinition, config);
                break;                
                }
            default:
                {
                error = KErrNotSupported;
                }
            }
        }


    return error;    
    }
    
TInt COmxILDummyAudioDecILIF::GetExtConfig(const TDesC8& aParameterName, 
                                           TILStruct& aConfig, 
                                           const MILComponentPortIf* aComponentPort) const
    {
    TInt portIndex = 0xFF; // Initialize to invalid port index
    if (aComponentPort)
	    {
    	portIndex = aComponentPort->PortIndex();
	    }
    else
		{
		// All parameters and configs are targetted to the port.
		return KErrArgument;
		}

    OMX_INDEXTYPE indexType;

    TInt error = KErrNone;
    error = OmxGetExtensionIndex(aParameterName, &indexType);

    if (error == KErrNone)
        {
        switch (aConfig.Uid().iUid)
            {
            case KUidTPortDefinitionType:
                {
                OMX_PARAM_PORTDEFINITIONTYPE* config = TILPortDefinition::GetStructure(aConfig);    
	            ASSERT(config);
                config->nVersion = KOMXILSpecVersion;
                config->nSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
                config->nPortIndex = portIndex;
                error = OmxGetParameter(indexType, config);
                break;
                }
            default:
                {
                error = KErrNotSupported;
                }
            }
        }
    return error;    
    }

TInt COmxILDummyAudioDecILIF::SetComponentIfRole(const TUid& aComponentRole)
    {
    if ((aComponentRole == KUidFormatAAC) ||
        (aComponentRole == KUidFormatOGG) ||
        (aComponentRole == KUidFormatPCM16))
        {
        return (COmxILDummyBaseILIF::SetComponentIfRole(aComponentRole));
        }
    else
        {
        return KErrNotSupported;
        }
    }
