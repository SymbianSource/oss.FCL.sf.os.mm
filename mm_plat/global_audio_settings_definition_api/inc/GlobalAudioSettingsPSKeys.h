/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Global Audio Settings Publish & Subscribe keys (Domain API)
*                specifying the settings of global audio settings.
*
*/


#ifndef GLOBALAUDIOSETTINGSPSKEYS_H
#define GLOBALAUDIOSETTINGSPSKEYS_H

// INCLUDES
#include <e32def.h>

// CONSTANTS

/**
* Global Audio Settings Publish & Subscribe UID.
*/
const TUid KGASPSUidGlobalAudioSettings = {0x10207B7C};

/**
* This key is required for the Global Audio Settings to determine whether the
* warning tones in the current profiles are enabled or not.
*
* Possible integer values:
* 0 = "Warning Tones Disabled". Warning tones in the current profile are disabled.
* 1 = "Warning Tones Enabled". Warning tones in the current profile are enabled.
*/
const TUint32 KGASWarningTones = 0x00000001;

/**
* This key is required for the Global Audio Settings to determine whether the
* message tones in the current profiles are enabled or not.
*
* Possible integer values:
* 0 = "Message Tones Disabled". Message tones in the current profile are disabled.
* 1 = "Message Tones Enabled". Message tones in the current profile are enabled.
*/
const TUint32 KGASMessageTones = 0x00000002;

/**
* This key is required for the Global Audio Settings to determine whether the
* current profiles selected in the phone is silent profile or not.
*
* Possible integer values:
* 0 = "Not Silent Profile". Current profile is not silent profile.
* 1 = "Silent Profile". Current profile is silent profile.
*/
const TUint32 KGASSilentProfile = 0x00000003;

/**
* This key is required for the Global Audio Settings to determine whether the
* Vibra in the current profiles are enabled or not.
*
* Possible integer values:
* 0 = "Vibra Disabled". Vibra in the current profile are disabled.
* 1 = "Vibra Enabled". Vibra in the current profile are enabled.
*/
const TUint32 KGASVibra = 0x00000004;

/**
* This key is required for the Global Audio Settings to determine whether the
* PublicSilence selected in the phone or not.
*
* Possible integer values:
* 0 = "Not PublicSilence". PublicSilence not selected in the phone.
* 1 = "PublicSilence". PublicSilence selected in the phone
*/
const TUint32 KGASPublicSilence = 0x00000005;

#endif      // GLOBALAUDIOSETTINGSPSKEYS_H

// End of file
