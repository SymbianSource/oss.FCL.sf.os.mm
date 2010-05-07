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

#ifndef OMXILSYMBIANADPCMDECODEREXTENSIONS_H
#define OMXILSYMBIANADPCMDECODEREXTENSIONS_H

#include <openmax/il/khronos/v1_x/OMX_Types.h>

/**
 * Current version of the AdPcm decoder config.
 */
const OMX_VERSIONTYPE KAdPcmDecoderConfigVersion = { 1, 0, 0, 0 };

/**
 * Custom index used by Symbian's ADPCM Decoder to set the size of AdPcm blocks
 * @C OMX_SYMBIAN_AUDIO_PARAM_ADPCMDECODER_SAMPLESPERBLOCK .
 */
#define OMX_SymbianIndexParamAudioAdPcmDecoderBlockAlign 0x7FD19E5D

/**
 * Custom OpenMAX IL structure for the AdPcm decoder.
 * Use with ID @C OMX_SymbianIndexParamAudioAdPcmDecoderSamplesPerBlock .
 * Sets the size of AdPcm blocks.
 */
struct OMX_SYMBIAN_AUDIO_PARAM_ADPCMDECODER_BLOCKALIGN
	{
	/**
	 * Set to the size of this structure, in bytes.
	 */
	OMX_U32 nSize;

	/**
	* Set to OMX specification version information.
	*/
	OMX_VERSIONTYPE nVersion;

	/**< Port that this structure applies to */
	OMX_U32 nPortIndex;            
	
	/**
	* Sets the size of AdPcm blocks.
	*/
	OMX_U32 nBlockAlign;
	};
#endif //OMXILSYMBIANADPCMDECODEREXTENSIONS_H
