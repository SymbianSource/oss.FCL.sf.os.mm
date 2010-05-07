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

#ifndef ILTYPEUIDS_H
#define ILTYPEUIDS_H

/**
@file
@internalTechnology
@prototype
*/

/**
The UID to identify the specific TOMXILStructParam type of IL structs.
@see TOMXILStructParam
*/
const TInt KUidOMXILStructType =  0x10285C09;

/**
The UID to identify the TTaskConfig encapsulated in the TILStruct.
@see TILTaskConfig
*/
const TInt KUidMMFTTaskConfig = 0x10285C0A;

/**
The UID to identify the TAudioSpecificConfigType encapsulated in the TILStruct.
@see TILAudioSpecificConfig
*/
const TInt KUidMMFAudioSpecificConfigType = 0x10285C7E;

/**
The UID to identify the TTimeIntervalMicroSeconds encapsulated in the TILStruct.
@see TILVolumeRamp
*/
const TInt KUidMMFVolumeRampType = 0x10285CC7;

/**
The UID to identify the TUint encapsulated in the TILStruct representing the Volume.
@see TILVolume
*/
const TInt KUidMMFVolumeType = 0x10285D18;

/**
The UID to identify the TUint encapsulated in the TILStruct representing the number of bytes played.
@see TILBytesPlayed
*/
const TInt KUidMMFBytesPlayedType = 0x10285CC8;

/**
The UID to identify the TPortDefinitionType encapsulated in the TILStruct.
@see TILPortDefinition
*/
const TInt KUidTPortDefinitionType =  0x10285C0B;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* The rest of this file defines some UIDs that are not publishedPartner yet.
/* They have been defined for internal used only as a proof of concept for testing
/* the varios possible MILIF implementations.
/* These definition could be upgraded to publishedPartner when the need for Symbian 
/* support of these particular params arises.
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
The UID to identify the TAacConfig encapsulated in the TILStruct.
@see TILAacConfig

@internalTechnology
@prototype
*/
const TInt KUidMMFTAacConfig = 0x10285C25;

/**
The UID to identify the TAudioPortFormatType encapsulated in the TILStruct.
@see TILAudioPortFormat

@internalTechnology
@prototype
*/
const TInt KUidTAudioPortFormatType =  0x10285C0C;

/**
The UID to identify the TPortVorbisType encapsulated in the TILStruct.
@see TILPortVorbis

@internalTechnology
@prototype
*/
const TInt KUidTPortVorbisType =  0x10285C0D;

/**
The UID to identify the TPortPCMModeType encapsulated in the TILStruct.
@see TILPortModePCM

@internalTechnology
@prototype
*/
const TInt KUidTPortPCMModeType =  0x10285C0E;

/**
The UID to identify the TPortAACProfileType encapsulated in the TILStruct.
@see TILPortAACProfile

@internalTechnology
@prototype
*/
const TInt KUidTPortAACProfileType =  0x10285C0F;

/**
The UID to identify the TPortVolumeType encapsulated in the TILStruct.
@see TILPortVolume

@internalTechnology
@prototype
*/
const TInt KUidTPortVolumeType =  0x10285C10;

/**
The UID to identify the TPortMuteType encapsulated in the TILStruct.
@see TILPortMute

@internalTechnology
@prototype
*/
const TInt KUidTPortMuteType =  0x10285C11;


#endif // ILTYPEUIDS_H