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

#ifndef ILAUDIOCONFIGSTRUCTS_H
#define ILAUDIOCONFIGSTRUCTS_H

#include <openmax/il/khronos/v1_x/OMX_Audio.h>

/**
@file
@internalTechnology
@prototype
*/

/** 
This is the structure to be used by implementations of the MMMFDevSoundCustomInterfaceSpecificConfig::SetFormatSpecificConfigL() 
method to pass the AudioSpecificConfig configuration data blindly to the codec.
*/	
class TAudioSpecificConfigType
{
public:
	/** The pointer to the AudioSpecificConfig chunk of data.
    */
	TUint8* iAudioSpecificConfig;

	/** The length of the data encapsulated in this structure..
    */
	TUint32 iAudioSpecificConfigLength;
	
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* The rest of this file defines some structures that are not publishedPartner yet.
/* They have been defined for internal used only as a proof of concept for testing
/* the varios possible MILIF implementations.
/* These definition could be upgraded to publishedPartner when the need for Symbian 
/* support of these particular params arises.
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/** 
A typedef to symbianize the OpenMAX IL PORTFORMATTYPE parameter structure.
*/	
typedef OMX_AUDIO_PARAM_PORTFORMATTYPE	TAudioPortFormatType;

	
/**
A typedef to symbianize the OpenMAX IL VORBISTYPE parameter structure.

@internalTechnology
@prototype
*/	
typedef OMX_AUDIO_PARAM_VORBISTYPE		TPortVorbisType;

/** 
A typedef to symbianize the OpenMAX IL PCMMODETYPE parameter structure.

@internalTechnology
@prototype
*/	
typedef OMX_AUDIO_PARAM_PCMMODETYPE		TPortPCMModeType;

/** 
A typedef to symbianize the OpenMAX IL AACPROFILETYPE parameter structure.

@internalTechnology
@prototype
*/	
typedef OMX_AUDIO_PARAM_AACPROFILETYPE	TPortAACProfileType;

/** 
A typedef to symbianize the OpenMAX IL VOLUMETYPE parameter structure.

@internalTechnology
@prototype
*/	
typedef OMX_AUDIO_CONFIG_VOLUMETYPE		TPortVolumeType;

/** 
A typedef to symbianize the OpenMAX IL MUTETYPE parameter structure.

@internalTechnology
@prototype
*/	
typedef OMX_AUDIO_CONFIG_MUTETYPE		TPortMuteType;


/**
This is the structure to be used by implementations of the MMMFDevSoundCustomInterfaceSpecificConfig::SetFormatSpecificConfigL() 
method to extract configuration data from the AudioSpecificConfig.

@internalTechnology
@prototype
*/
class TAacConfig
	{
public:
	inline TAacConfig();

	inline TAacConfig(TUint8 aAudioType, TUint8 aSamplingRateIndex, TUint32 aNumChannels, TInt aStereoMode);

public:
	/** The audio object type
    */
	TUint8 iAudioType;

	/** The rate index of the audio data.
    */
	TUint8 iSamplingRateIndex;

	/** Number of channels of the underlying AAC coded stream
	*/
	TUint32 iNumChannels;

	/** The type of stereo audio data.
	*/
	TInt iStereoMode;
	};

inline
TAacConfig::TAacConfig()
	:
	iAudioType(0), 
	iSamplingRateIndex(0),
	iNumChannels(0),
	iStereoMode(0)
	{
	}

inline
TAacConfig::TAacConfig(
	TUint8 aAudioType,
	TUint8 aSamplingRateIndex,
	TUint32 aNumChannels,
	TInt aStereoMode
	)
	:
	iAudioType(aAudioType),
	iSamplingRateIndex(aSamplingRateIndex),
	iNumChannels(aNumChannels),
	iStereoMode(aStereoMode)
	{
	}



#endif  // ILAUDIOCONFIGSTRUCTS_H
