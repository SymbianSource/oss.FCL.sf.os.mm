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
 @publishedPartner@prototype
*/

#ifndef OMXILSYMBIANAUDIOPCMEXTENSIONS_H
#define OMXILSYMBIANAUDIOPCMEXTENSIONS_H


#include <openmax/il/khronos/v1_x/OMX_Types.h>


/**
 * The string that the Symbian's OpenMAX IL PCM Renderer component will
 * translate into a 32-bit OpenMAX IL index (@see
 * OMX_SYMBIAN_AUDIO_CONFIG_PCM_VOLUMERAMP) to support the
 * Volume Ramp feature of the MPlayCustomInterface CI
 */
const char sOmxSymbianPcmVolumeRamp [] =
	"OMX.Symbian.index.config.audio.pcm.volumeramp";


/**
 * Custom index used by Symbian's OpenMAX IL PCM renderer to select the
 * structure for setting/getting a Volume Ramp configuration data item
 */
#define OMX_SymbianIndexConfigAudioPcmVolumeRamp 0x7F000002


/**
 * Custom OpenMAX IL structure to be used as a container for an
 * Volume Ramp configuration data item
 */
struct OMX_SYMBIAN_AUDIO_CONFIG_PCM_VOLUMERAMP
	{
    OMX_U32 nSize;                 /**< Size of this structure, in Bytes */
    OMX_VERSIONTYPE nVersion;      /**< OMX specification version information */
    OMX_U32 nPortIndex;            /**< Port that this structure applies to */
    OMX_U64 nRampDuration;         /**< the period, in microseconds, over which the volume level is 
    								   to rise smoothly from nothing to the 
    								   required volume level */
	};

/**
 * The string that the Symbian's OpenMAX IL PCM Renderer component will
 * translate into a 32-bit OpenMAX IL index (@see
 * OMX_SYMBIAN_AUDIO_CONFIG_PCM_BYTESPLAYED) to support the
 * Bytes Played feature of the MPlayCustomInterface CI
 */
const char sOmxSymbianPcmBytesPlayed [] =
	"OMX.Symbian.index.config.audio.pcm.bytesplayed";


/**
 * Custom index used by Symbian's OpenMAX IL PCM renderer to select the
 * structure for getting a Bytes Played configuration data item
 */
#define OMX_SymbianIndexConfigAudioBytesPlayed 0x7F000003


/**
 * Custom OpenMAX IL structure to be used as a container for an
 * Bytes Played configuration data item
 */
struct OMX_SYMBIAN_AUDIO_CONFIG_PCM_BYTESPLAYED
	{
    OMX_U32 nSize;                 /**< Size of this structure, in Bytes */
    OMX_VERSIONTYPE nVersion;      /**< OMX specification version information */
    OMX_U32 nBytesPlayed;          /**< the number of bytes played */
	};


#endif // OMXILSYMBIANAUDIOPCMEXTENSIONS_H
