/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   This file contains definitions of GlobalAudioSettings.
*
*/




#ifndef GLOBALAUDIOSETTINGSDATA_H
#define GLOBALAUDIOSETTINGSDATA_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  This class defines the GlobalAudioSettings data structure.
*
*  @lib GlobalAudioSettings.lib
*  @since 3.2
*/
class TGlobalAudioSettings
    {
    public:

        /**
        * Constructor.
        */
        TGlobalAudioSettings() : iWarningTones(EFalse),iMessageTones(EFalse),iSilentProfile(EFalse),
                                 iVibra(EFalse),iPublicSilence(EFalse)
        {}

		TBool iWarningTones;
		TBool iMessageTones;
		TBool iSilentProfile;
		TBool iVibra;
		TBool iPublicSilence;
    };

#endif      // GlobalAudioSETTINGSDATA_H

// End of File
