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

/**
 @file
 @internalTechnology
 @prototype
*/

#ifndef TILSTRUCTTYPES_H
#define TILSTRUCTTYPES_H

#include "tilstruct.h"
#include "iltypeuids.h"
#include "ilconfigstructs.h"
#include "ilaudioconfigstructs.h"
#include <mmf/server/taskconfig.h>


/**
TILStruct to encapsulate TTaskConfig 
see KUidMMFTTaskConfig
see TILStructParam
*/
typedef TILStructParam<KUidMMFTTaskConfig, TTaskConfig> TILTaskConfig;

/**
TILStruct to encapsulate TAudioSpecificConfigType 
see KUidMMFAudioSpecificConfigType
see TILStructParam
*/
typedef TILStructParam<KUidMMFAudioSpecificConfigType, TAudioSpecificConfigType> TILAudioSpecificConfig;

/**
TILStruct to encapsulate TPortDefinitionType 
see KUidTPortDefinitionType
see TILStructParam
*/
typedef TILStructParam<KUidTPortDefinitionType, TPortDefinitionType> TILPortDefinition;

/**
TILStruct to encapsulate TTimeIntervalMicroSeconds containing the volume ramping interval
see KUidMMFVorlumeRampType
see TILStructParam
*/
typedef TILStructParam<KUidMMFVolumeRampType, TTimeIntervalMicroSeconds> TILVolumeRamp;

/**
TILStruct to encapsulate the TUint containing the volume setting
see KUidMMFVolumeType
see TILStructParam
*/
typedef TILStructParam<KUidMMFVolumeType, TUint> TILVolume;

/**
TILStruct to encapsulate the TUint containing the number of bytes played
see KUidMMFBytesPlayedType
see TILStructParam
*/
typedef TILStructParam<KUidMMFBytesPlayedType, TUint> TILBytesPlayed;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* The rest of this file defines some structures that are not publishedPartner yet.
/* They have been defined for internal used only as a proof of concept for testing
/* the various possible MILIF implementations.
/* These definition could be upgraded to publishedPartner when the need for Symbian 
/* support of these particular params arises.
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/**
TILStruct to encapsulate TAacConfig 
see KUidMMFTAacConfig
see TILStructParam

@internalTechnology
@prototype
*/
typedef TILStructParam<KUidMMFTAacConfig, TAacConfig> TILAacConfig;

/**
TILStruct to encapsulate TAudioPortFormatType 
see KUidTAudioPortFormatType
see TILStructParam

@internalTechnology
@prototype
*/
typedef TILStructParam<KUidTAudioPortFormatType, TAudioPortFormatType> TILAudioPortFormat;

/**
TILStruct to encapsulate TPortVorbisType 
see KUidTPortVorbisType
see TILStructParam

@internalTechnology
@prototype
*/
typedef TILStructParam<KUidTPortVorbisType, TPortVorbisType> TILPortVorbis;

/**
TILStruct to encapsulate TPortPCMModeType 
see KUidTPortPCMModeType
see TILStructParam

@internalTechnology
@prototype
*/
typedef TILStructParam<KUidTPortPCMModeType, TPortPCMModeType> TILPortPCMMode;

/**
TILStruct to encapsulate TPortAACProfileType 
see KUidTPortAACProfileType
see TILStructParam

@internalTechnology
@prototype
*/
typedef TILStructParam<KUidTPortAACProfileType, TPortAACProfileType> TILPortAACProfile;

/**
TILStruct to encapsulate TPortVolumeType 
see KUidTPortVolumeType
see TILStructParam

@internalTechnology
@prototype
*/
typedef TILStructParam<KUidTPortVolumeType, TPortVolumeType> TILPortVolume;

/**
TILStruct to encapsulate TPortMuteType 
see KUidTPortMuteType
see TILStructParam

@internalTechnology
@prototype
*/
typedef TILStructParam<KUidTPortMuteType, TPortMuteType> TILPortMute;

#endif // TILSTRUCTTYPES_H