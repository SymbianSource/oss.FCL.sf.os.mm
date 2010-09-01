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




#ifndef GLOBALAUDIOSETTINGSIMPL_H
#define GLOBALAUDIOSETTINGSIMPL_H

#include <e32property.h>
#include <e32base.h>
#include <e32debug.h>
#include "GlobalAudioSettingsData.h"
#include <GlobalAudioSettings.h>
#include <MAudioClientsListObserver.h>

class MAudioSettingsObserver;
//class CGlobalAudioSettings;
class CWarningTonesObserverAO;
class CMessagingTonesObserverAO;
class CSilentProfileObserverAO;
class CVibraObserverAO;
class CAudioClientsListManagerAO;


/**
*  Defines functions that client uses to set phone profile settings.
*
*  @lib GlobalAudioSettings.lib
*  @since Series 60 3.2
*/
class CGlobalAudioSettingsImpl: public CBase
    {
    public:
        /**
        * function for creating the GlobalAudioSettingsImpl
        *
        */
        IMPORT_C static CGlobalAudioSettingsImpl* NewL(
                        CGlobalAudioSettings& aGlobalAudioSettings,
                        MAudioSettingsObserver& aAudioSettingsObserver);
        /**
        * Destructor.
        *
        */
        ~CGlobalAudioSettingsImpl();
        /**
        * Returns warning tones status.
        *
        * @since Series 60 3.2
        * @return TBool aEnable. ETrue if warning tones are enabled else EFalse.
        */
        IMPORT_C TBool IsWarningTonesEnabled();
        /**
        * Returns message tones status.
        *
        * @since Series 60 3.2
        * @return TBool aEnable. ETrue if message tones are enabled else EFalse.
        */
        IMPORT_C TBool IsMessageTonesEnabled();
        /**
        * Returns silent profile status.
        *
        * @since Series 60 3.2
        * @return TBool aEnable. ETrue if current profile is silent profile else EFalse.
        */
        IMPORT_C TBool IsSilentProfileEnabled();
        /**
        * Returns vibra is enabled or not in the current profile status.
        *
        * @since Series 60 3.2
        * @return TBool aEnable. ETrue if current profile vibra active else EFalse.
        */
        IMPORT_C TBool IsVibraEnabled();
        /**
        * Returns PublicSilence is enabled or not status.
        *
        * @since Series 60 3.2
        * @return TBool aEnable. ETrue if PublicSilence active else EFalse.
        */
        IMPORT_C TBool IsPublicSilenceEnabled();

        /**
        * Registers audio clients list observer.
        *
        * When registered successfully, the observer will start receiveing
        * callback defined in MAudioClientsListObserver when audio clients list
        * changes.
        *
        * @since S60 3.2
        * @param MAudioClientsListObserver& aObserver. A reference to class
        *           implementing MAudioClientsListObserver interface.
        * @return TInt. KErrNone if successful. KErrAlreadyExists if the observer
        *           is already registered.
        */
        TInt RegisterAudioClientsListObserver(
                                    MAudioClientsListObserver& aObserver );
                
        /**
        * Unregisters audio clients list observer.
        *
        * When unregistered, the observer will no longer receive callback when
        * audio clients list changes.
        *
        * @since S60 3.2
        * @param MAudioClientsListObserver& aObserver. A reference to class
        *           implementing MAudioClientsListObserver interface.
        * @return TInt. KErrNone if successful. KErrNotFound if the observer
        *           is not registered.
        */
        TInt UnregisterAudioClientsListObserver(
                                    MAudioClientsListObserver& aObserver);

        /**
        * Gets audio clients list.
        *
        * On return of this function, the parameter aList will contain active
        * audio client application process ids.
        *
        * The parameter aList will be reset in the case
        *
        * @since S60 3.2
        * @param TAudioClientListType aFilter. Type of client list.
        * @param RArray<TProcessId>& aList. A reference to RArray.
        * @return TInt. KErrNone if successful. KErrBadName if
        *           aType is not one of the TAudioClientListType enumeration.
        *           Otherwise a systemwide error code.
        *           In case of error, the array aList will be reset.
        */
        TInt GetAudioClientsList(
                                CGlobalAudioSettings::TAudioClientListType aType,
                                RArray<TProcessId>& aList );

    
    private:
        CGlobalAudioSettingsImpl(CGlobalAudioSettings &aGlobalAudioSettings,
                                 MAudioSettingsObserver& aAudioSettingsObserver);
        /**
        * Second Phase Costructor for GlobalAudioSettingsImpl Instance
        * @since 3.2
        * @param void
        */
        void ConstructL();
            
    private:
        TGlobalAudioSettings iGlobalAudioSettingsData;
        CGlobalAudioSettings& iGlobalAudioSettings;
        MAudioSettingsObserver&   iAudioSettingsObserver;
        CWarningTonesObserverAO* iWarningTonesObserverAO;
        CMessagingTonesObserverAO* iMessagingTonesObserverAO;
        CSilentProfileObserverAO* iSilentProfileObserverAO;
        CVibraObserverAO* iVibraObserverAO;
        // Object managing Audio Clients List implementation
        CAudioClientsListManagerAO* iCAudioClientsListManagerAO;
        // Object managing Paused Clients List implementation
        CAudioClientsListManagerAO* iPausedClientsListManagerAO;              
        // List of Audio Clients List Observer objects
        RPointerArray<MAudioClientsListObserver> iAudioClientsListObserverArray;
    };


class CWarningTonesObserverAO : public CActive
	{
public:
    // Ist Phase constr and Destr
	static CWarningTonesObserverAO* NewL(CGlobalAudioSettings &aGlobalAudioSettings,
                                         MAudioSettingsObserver& aAudioSettingsObserver,
                                         TGlobalAudioSettings& aGlobalAudioSettingsData);
	~CWarningTonesObserverAO();
	void Subscribe();

protected:
	// From CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);

private:
	// Constr and IInd phase constr
	CWarningTonesObserverAO(CGlobalAudioSettings &aGlobalAudioSettings,
                                                 MAudioSettingsObserver& aAudioSettingsObserver,
                                                 TGlobalAudioSettings& aGlobalAudioSettingsData);
	void ConstructL();
	CGlobalAudioSettings& iGlobalAudioSettings;
    MAudioSettingsObserver&   iAudioSettingsObserver;
    RProperty iWarningTonesProperty;
    TGlobalAudioSettings& iGlobalAudioSettingsData;
	};

class CMessagingTonesObserverAO : public CActive
	{
public:
    // Ist Phase constr and Destr
	static CMessagingTonesObserverAO* NewL(CGlobalAudioSettings &aGlobalAudioSettings,
                                         MAudioSettingsObserver& aAudioSettingsObserver,
                                         TGlobalAudioSettings& aGlobalAudioSettingsData);
	~CMessagingTonesObserverAO();
	void Subscribe();

protected:
	// From CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);

private:
	// Constr and IInd phase constr
	CMessagingTonesObserverAO(CGlobalAudioSettings &aGlobalAudioSettings,
                                                 MAudioSettingsObserver& aAudioSettingsObserver,
                                                 TGlobalAudioSettings& aGlobalAudioSettingsData);
	void ConstructL();
	CGlobalAudioSettings& iGlobalAudioSettings;
    MAudioSettingsObserver&   iAudioSettingsObserver;
    RProperty iMessagingTonesProperty;
    TGlobalAudioSettings& iGlobalAudioSettingsData;
	};
	
class CSilentProfileObserverAO : public CActive
	{
public:
    // Ist Phase constr and Destr
	static CSilentProfileObserverAO* NewL(CGlobalAudioSettings &aGlobalAudioSettings,
                                         MAudioSettingsObserver& aAudioSettingsObserver,
                                         TGlobalAudioSettings& aGlobalAudioSettingsData);
	~CSilentProfileObserverAO();
	void Subscribe();

protected:
	// From CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);

private:
	// Constr and IInd phase constr
	CSilentProfileObserverAO(CGlobalAudioSettings &aGlobalAudioSettings,
                                                 MAudioSettingsObserver& aAudioSettingsObserver,
                                                 TGlobalAudioSettings& aGlobalAudioSettingsData);
	void ConstructL();
	CGlobalAudioSettings& iGlobalAudioSettings;
    MAudioSettingsObserver&   iAudioSettingsObserver;
    RProperty iSilentProfileProperty;
    TGlobalAudioSettings& iGlobalAudioSettingsData;
	};

class CVibraObserverAO : public CActive
	{
public:
    // Ist Phase constr and Destr
	static CVibraObserverAO* NewL(CGlobalAudioSettings &aGlobalAudioSettings,
                                         MAudioSettingsObserver& aAudioSettingsObserver,
                                         TGlobalAudioSettings& aGlobalAudioSettingsData);
	~CVibraObserverAO();
	void Subscribe();

protected:
	// From CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);

private:
	// Constr and IInd phase constr
	CVibraObserverAO(CGlobalAudioSettings &aGlobalAudioSettings,
                                                 MAudioSettingsObserver& aAudioSettingsObserver,
                                                 TGlobalAudioSettings& aGlobalAudioSettingsData);
	void ConstructL();
	CGlobalAudioSettings& iGlobalAudioSettings;
    MAudioSettingsObserver&   iAudioSettingsObserver;
    RProperty iVibraProperty;
    TGlobalAudioSettings& iGlobalAudioSettingsData;
	};

#endif // GLOBALAUDIOSETTINGSIMPL_H

// End of file
