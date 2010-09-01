/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Implementation of the Phone Profile Settings API
*
*/




// INCLUDE FILES
#include "GlobalAudioSettingsImpl.h"
#include <e32std.h>
#include <GlobalAudioSettingsPSKeys.h>
#include <MGlobalAudioSettingsObserver.h>
#include <centralrepository.h>
#include "AudioClientsListManagerAO.h"
#include <AudioClientsListPSKeys.h>

#ifdef _DEBUG
#define PRINT_MESSAGE
#endif // _DEBUG

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CGlobalAudioSettingsImpl::NewL();
// 
// -----------------------------------------------------------------------------
//
CGlobalAudioSettingsImpl::CGlobalAudioSettingsImpl(
                            CGlobalAudioSettings &aGlobalAudioSettings,
                            MAudioSettingsObserver& aAudioSettingsObserver)
:iGlobalAudioSettings(aGlobalAudioSettings),
 iAudioSettingsObserver(aAudioSettingsObserver)
	{
	}
	
// -----------------------------------------------------------------------------
// CGlobalAudioSettingsImpl::NewL();
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CGlobalAudioSettingsImpl* CGlobalAudioSettingsImpl::NewL(
                                        CGlobalAudioSettings &aGlobalAudioSettings,
                                        MAudioSettingsObserver& aAudioSettingsObserver)
	{
	CGlobalAudioSettingsImpl* self = new (ELeave) CGlobalAudioSettingsImpl(
	                                    aGlobalAudioSettings,aAudioSettingsObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
	}
	
// -----------------------------------------------------------------------------
// CGlobalAudioSettingsImpl::ConstructL();
// 
// -----------------------------------------------------------------------------
//
void CGlobalAudioSettingsImpl::ConstructL()
	{
	iWarningTonesObserverAO = CWarningTonesObserverAO::NewL(
                	                                        iGlobalAudioSettings,
                	                                        iAudioSettingsObserver,
                	                                        iGlobalAudioSettingsData);
	iWarningTonesObserverAO->Subscribe();
	
	iMessagingTonesObserverAO = CMessagingTonesObserverAO::NewL(
                                        	                iGlobalAudioSettings,
                                        	                iAudioSettingsObserver,
                                        	                iGlobalAudioSettingsData);
	iMessagingTonesObserverAO->Subscribe();

	iSilentProfileObserverAO = CSilentProfileObserverAO::NewL(
                                        	                iGlobalAudioSettings,
                                        	                iAudioSettingsObserver,
                                        	                iGlobalAudioSettingsData);
	iSilentProfileObserverAO->Subscribe();

	iVibraObserverAO = CVibraObserverAO::NewL(
                        	                iGlobalAudioSettings,
                        	                iAudioSettingsObserver,
                        	                iGlobalAudioSettingsData);
	iVibraObserverAO->Subscribe();

    RProperty publicSilenceProperty;
	User::LeaveIfError(publicSilenceProperty.Attach(KGASPSUidGlobalAudioSettings, KGASPublicSilence));
	User::LeaveIfError(publicSilenceProperty.Get(iGlobalAudioSettingsData.iPublicSilence));
	publicSilenceProperty.Close();

    iCAudioClientsListManagerAO = CAudioClientsListManagerAO::NewL( iGlobalAudioSettings,iAudioClientsListObserverArray,KAudioPolicyAudioClients);
            
    iPausedClientsListManagerAO = CAudioClientsListManagerAO::NewL( iGlobalAudioSettings,iAudioClientsListObserverArray,KAudioPolicyApplicationAudioStatePaused );
    }
    
// Destructor
EXPORT_C CGlobalAudioSettingsImpl::~CGlobalAudioSettingsImpl()
    {
        delete iWarningTonesObserverAO;
        delete iMessagingTonesObserverAO;
        delete iSilentProfileObserverAO;
        delete iVibraObserverAO;
        delete iCAudioClientsListManagerAO;
        delete iPausedClientsListManagerAO;
        iAudioClientsListObserverArray.Close();
    }

// -----------------------------------------------------------------------------
// CGlobalAudioSettingsImpl::NewL
// Static function for creating an instance of the EnvironmentalReverb object.
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CGlobalAudioSettingsImpl::IsWarningTonesEnabled()
	{
	if(iGlobalAudioSettingsData.iWarningTones)
	    return(ETrue);
	else
	    return(EFalse);
	}

// -----------------------------------------------------------------------------
// CGlobalAudioSettingsImpl::IsMessageTonesEnabled
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CGlobalAudioSettingsImpl::IsMessageTonesEnabled()
	{
	if(iGlobalAudioSettingsData.iMessageTones)
	    return(ETrue);
	else
	    return(EFalse);
	}

// -----------------------------------------------------------------------------
// CGlobalAudioSettingsImpl::IsSilentProfileEnabled
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CGlobalAudioSettingsImpl::IsSilentProfileEnabled()
	{
	if(iGlobalAudioSettingsData.iSilentProfile)
	    return(ETrue);
	else
	    return(EFalse);
	}

// -----------------------------------------------------------------------------
// CGlobalAudioSettingsImpl::IsVibraEnabled
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CGlobalAudioSettingsImpl::IsVibraEnabled()
	{
	if(iGlobalAudioSettingsData.iVibra)
	    return(ETrue);
	else
	    return(EFalse);
	}

// -----------------------------------------------------------------------------
// CGlobalAudioSettingsImpl::IsPublicSilenceEnabled
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CGlobalAudioSettingsImpl::IsPublicSilenceEnabled()
	{
	if(iGlobalAudioSettingsData.iPublicSilence)
	    return(ETrue);
	else
	    return(EFalse);
	}

// -----------------------------------------------------------------------------
// CGlobalAudioSettingsImpl::RegisterAudioClientsListObserver
// 
// -----------------------------------------------------------------------------
//
TInt CGlobalAudioSettingsImpl::RegisterAudioClientsListObserver(
                            MAudioClientsListObserver& aObserver )
	{
	TInt status(KErrAlreadyExists);
	if ( iAudioClientsListObserverArray.Find( &aObserver ) == KErrNotFound )
	    {
	    status = iAudioClientsListObserverArray.Append( &aObserver);
	    if ( ( status == KErrNone ) && (iAudioClientsListObserverArray.Count() == 1 ) )
	        {  	       
	 	    iCAudioClientsListManagerAO->MonitorStart();
	        iPausedClientsListManagerAO->MonitorStart();
	        }
	    }
	return status;
	}
        
// -----------------------------------------------------------------------------
// CGlobalAudioSettingsImpl::UnregisterAudioClientsListObserver
// 
// -----------------------------------------------------------------------------
//
TInt CGlobalAudioSettingsImpl::UnregisterAudioClientsListObserver(
                            MAudioClientsListObserver& aObserver)
	{
	TInt status(KErrNone);
	status = iAudioClientsListObserverArray.Find( &aObserver );
	if ( status != KErrNotFound )
	    {
	    iAudioClientsListObserverArray.Remove( status );
	    if ( iAudioClientsListObserverArray.Count() == 0 )
	        {
	        iCAudioClientsListManagerAO->MonitorStop();
	        iPausedClientsListManagerAO->MonitorStop();	
	        }
	    }
	return status;
	}

// -----------------------------------------------------------------------------
// CGlobalAudioSettingsImpl::GetAudioClientsList
// 
// -----------------------------------------------------------------------------
//
TInt CGlobalAudioSettingsImpl::GetAudioClientsList(
                        CGlobalAudioSettings::TAudioClientListType aType,
                        RArray<TProcessId>& aList )
	{
	TInt status(KErrNone);
	
	switch(aType)
		{
		case CGlobalAudioSettings::EActiveAudioClients:
			{
			status = iCAudioClientsListManagerAO->GetAudioClientsList( aType, aList );	
			}
			break;
        case CGlobalAudioSettings::EPausedAudioClients:
        	{
        	status = iPausedClientsListManagerAO->GetAudioClientsList( aType, aList );     	
        	}
        	break;
		default:		
			break;
			
		}
	 return status;
	

	}

//////////////////////////////////////////////////////////////////////
//	CWarningTonesObserverAO											//
//////////////////////////////////////////////////////////////////////
CWarningTonesObserverAO::CWarningTonesObserverAO(
                            CGlobalAudioSettings &aGlobalAudioSettings,
                            MAudioSettingsObserver& aAudioSettingsObserver,
                            TGlobalAudioSettings& aGlobalAudioSettingsData)
:CActive(EPriorityStandard),
 iGlobalAudioSettings(aGlobalAudioSettings),
 iAudioSettingsObserver(aAudioSettingsObserver),
 iGlobalAudioSettingsData(aGlobalAudioSettingsData)
	{
	CActiveScheduler::Add(this);
	}

CWarningTonesObserverAO::~CWarningTonesObserverAO()
	{
	Cancel();
	iWarningTonesProperty.Close();
	}

CWarningTonesObserverAO* CWarningTonesObserverAO::NewL(
                        CGlobalAudioSettings &aGlobalAudioSettings,
                        MAudioSettingsObserver& aAudioSettingsObserver,
                        TGlobalAudioSettings& aGlobalAudioSettingsData)
	{
	CWarningTonesObserverAO* self = new (ELeave) CWarningTonesObserverAO(
	                                                    aGlobalAudioSettings, 
	                                                    aAudioSettingsObserver,
	                                                    aGlobalAudioSettingsData);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CWarningTonesObserverAO::ConstructL()
	{
	User::LeaveIfError(iWarningTonesProperty.Attach(KGASPSUidGlobalAudioSettings, KGASWarningTones));
    User::LeaveIfError(iWarningTonesProperty.Get(iGlobalAudioSettingsData.iWarningTones));
	}
	
void CWarningTonesObserverAO::Subscribe()
	{
	if (!IsActive())
	    {
    	SetActive();
    	iWarningTonesProperty.Subscribe(iStatus);
	    }
	}

void CWarningTonesObserverAO::RunL()
	{
	TInt status(iStatus.Int());
#ifdef PRINT_MESSAGE
    RDebug::Print(_L(" CWarningTonesObserverAO::RunL:iStatus[%d]"), status);
#endif // PRINT_MESSAGE
    if ( status == KErrNone )
        {
        Subscribe();
        status = iWarningTonesProperty.Get(iGlobalAudioSettingsData.iWarningTones);
        if(status == KErrNone)
            {
            iAudioSettingsObserver.SettingsChanged(iGlobalAudioSettings, MAudioSettingsObserver::KWarningTones );
            }
#ifdef PRINT_MESSAGE
        else
            {
            RDebug::Print(_L(" CWarningTonesObserverAO::RunL:Property.Get Error[%d]"), status);            
            }
#endif // PRINT_MESSAGE
        }
	}

void CWarningTonesObserverAO::DoCancel()
	{
	iWarningTonesProperty.Cancel();
	}

TInt CWarningTonesObserverAO::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}

//////////////////////////////////////////////////////////////////////
//	CMessagingTonesObserverAO											//
//////////////////////////////////////////////////////////////////////
CMessagingTonesObserverAO::CMessagingTonesObserverAO(
                                CGlobalAudioSettings &aGlobalAudioSettings,
                                MAudioSettingsObserver& aAudioSettingsObserver,
                                TGlobalAudioSettings& aGlobalAudioSettingsData)
:CActive(EPriorityStandard),
 iGlobalAudioSettings(aGlobalAudioSettings),
 iAudioSettingsObserver(aAudioSettingsObserver),
 iGlobalAudioSettingsData(aGlobalAudioSettingsData)
	{
	CActiveScheduler::Add(this);
	}

CMessagingTonesObserverAO::~CMessagingTonesObserverAO()
	{
	Cancel();
	iMessagingTonesProperty.Close();
	}

CMessagingTonesObserverAO* CMessagingTonesObserverAO::NewL(
                                               CGlobalAudioSettings &aGlobalAudioSettings,
                                               MAudioSettingsObserver& aAudioSettingsObserver,
                                               TGlobalAudioSettings& aGlobalAudioSettingsData)
	{
	CMessagingTonesObserverAO* self = new (ELeave) CMessagingTonesObserverAO(
                    	                        aGlobalAudioSettings, aAudioSettingsObserver, 
                    	                        aGlobalAudioSettingsData);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CMessagingTonesObserverAO::ConstructL()
	{
	User::LeaveIfError(iMessagingTonesProperty.Attach(KGASPSUidGlobalAudioSettings, KGASMessageTones));
	User::LeaveIfError(iMessagingTonesProperty.Get(iGlobalAudioSettingsData.iMessageTones));
	}
	
void CMessagingTonesObserverAO::Subscribe()
	{
	if (!IsActive())
	    {
    	SetActive();
    	iMessagingTonesProperty.Subscribe(iStatus);
	    }
	}

void CMessagingTonesObserverAO::RunL()
	{
	TInt status(iStatus.Int());
#ifdef PRINT_MESSAGE
    RDebug::Print(_L(" CMessagingTonesObserverAO::RunL:iStatus[%d]"), status);
#endif // PRINT_MESSAGE
    if ( status == KErrNone )
        {
        Subscribe();
        status = iMessagingTonesProperty.Get(iGlobalAudioSettingsData.iMessageTones);
        if(status == KErrNone)
            {
            iAudioSettingsObserver.SettingsChanged(iGlobalAudioSettings, MAudioSettingsObserver::KMessageTones);
            }
#ifdef PRINT_MESSAGE
        else
            {
            RDebug::Print(_L(" CMessagingTonesObserverAO::RunL:Property.Get Error[%d]"), status);
            }
#endif // PRINT_MESSAGE
        }
	}

void CMessagingTonesObserverAO::DoCancel()
	{
	iMessagingTonesProperty.Cancel();
	}

TInt CMessagingTonesObserverAO::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}
	
//////////////////////////////////////////////////////////////////////
//	CSilentProfileObserverAO											//
//////////////////////////////////////////////////////////////////////
CSilentProfileObserverAO::CSilentProfileObserverAO(
                                CGlobalAudioSettings &aGlobalAudioSettings,
                                MAudioSettingsObserver& aAudioSettingsObserver,
                                TGlobalAudioSettings& aGlobalAudioSettingsData)
:CActive(EPriorityStandard),
 iGlobalAudioSettings(aGlobalAudioSettings),
 iAudioSettingsObserver(aAudioSettingsObserver),
 iGlobalAudioSettingsData(aGlobalAudioSettingsData)
	{
	CActiveScheduler::Add(this);
	}

CSilentProfileObserverAO::~CSilentProfileObserverAO()
	{
	Cancel();
	iSilentProfileProperty.Close();
	}

CSilentProfileObserverAO* CSilentProfileObserverAO::NewL(
                                        CGlobalAudioSettings &aGlobalAudioSettings,
                                        MAudioSettingsObserver& aAudioSettingsObserver,
                                        TGlobalAudioSettings& aGlobalAudioSettingsData)
	{
	CSilentProfileObserverAO* self = new (ELeave) CSilentProfileObserverAO(
                    	                        aGlobalAudioSettings, aAudioSettingsObserver, 
                    	                        aGlobalAudioSettingsData);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CSilentProfileObserverAO::ConstructL()
	{
	User::LeaveIfError(iSilentProfileProperty.Attach(KGASPSUidGlobalAudioSettings, KGASSilentProfile));
	User::LeaveIfError(iSilentProfileProperty.Get(iGlobalAudioSettingsData.iSilentProfile));
	}
	
void CSilentProfileObserverAO::Subscribe()
	{
	if (!IsActive())
	    {
    	SetActive();
    	iSilentProfileProperty.Subscribe(iStatus);
	    }
	}

void CSilentProfileObserverAO::RunL()
	{
	TInt status(iStatus.Int());
#ifdef PRINT_MESSAGE
    RDebug::Print(_L(" CSilentProfileObserverAO::RunL:iStatus[%d]"), status);
#endif // PRINT_MESSAGE
    if ( status == KErrNone )
        {
        Subscribe();
        status = iSilentProfileProperty.Get(iGlobalAudioSettingsData.iSilentProfile);
        if(status == KErrNone)
            {
            iAudioSettingsObserver.SettingsChanged(iGlobalAudioSettings, MAudioSettingsObserver::KSilentProfile);
            }
#ifdef PRINT_MESSAGE
        else
            {
            RDebug::Print(_L(" CSilentProfileObserverAO::RunL:Property.Get Error[%d]"), status);
            }
#endif // PRINT_MESSAGE
        }
	}

void CSilentProfileObserverAO::DoCancel()
	{
	iSilentProfileProperty.Cancel();
	}

TInt CSilentProfileObserverAO::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}
	
//////////////////////////////////////////////////////////////////////
//	CVibraObserverAO											    //
//////////////////////////////////////////////////////////////////////
CVibraObserverAO::CVibraObserverAO(
                        CGlobalAudioSettings &aGlobalAudioSettings,
                        MAudioSettingsObserver& aAudioSettingsObserver,
                        TGlobalAudioSettings& aGlobalAudioSettingsData)
:CActive(EPriorityStandard),
 iGlobalAudioSettings(aGlobalAudioSettings),
 iAudioSettingsObserver(aAudioSettingsObserver),
 iGlobalAudioSettingsData(aGlobalAudioSettingsData)
	{
	CActiveScheduler::Add(this);
	}

CVibraObserverAO::~CVibraObserverAO()
	{
	Cancel();
	iVibraProperty.Close();
	}

CVibraObserverAO* CVibraObserverAO::NewL(
                            CGlobalAudioSettings &aGlobalAudioSettings,
                            MAudioSettingsObserver& aAudioSettingsObserver,
                            TGlobalAudioSettings& aGlobalAudioSettingsData)
	{
	CVibraObserverAO* self = new (ELeave) CVibraObserverAO(
                	                        aGlobalAudioSettings,
                	                        aAudioSettingsObserver, 
                	                        aGlobalAudioSettingsData);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CVibraObserverAO::ConstructL()
	{
	User::LeaveIfError(iVibraProperty.Attach(KGASPSUidGlobalAudioSettings, KGASVibra));
	User::LeaveIfError(iVibraProperty.Get(iGlobalAudioSettingsData.iVibra));
	}
	
void CVibraObserverAO::Subscribe()
	{
	if (!IsActive())
	    {
    	SetActive();
    	iVibraProperty.Subscribe(iStatus);
	    }
	}

void CVibraObserverAO::RunL()
	{
	TInt status(iStatus.Int());
#ifdef PRINT_MESSAGE
    RDebug::Print(_L(" CVibraObserverAO::RunL:iStatus[%d]"), status);
#endif // PRINT_MESSAGE
    if ( status == KErrNone )
        {
        Subscribe();
        status = iVibraProperty.Get(iGlobalAudioSettingsData.iVibra);
        if( status == KErrNone)
            {
            iAudioSettingsObserver.SettingsChanged(iGlobalAudioSettings, MAudioSettingsObserver::KVibra);
            }
#ifdef PRINT_MESSAGE
        else
            {
            RDebug::Print(_L(" CVibraObserverAO::RunL:Property.Get Error[%d]"), status);
            }
#endif // PRINT_MESSAGE
        }
	}

void CVibraObserverAO::DoCancel()
	{
	iVibraProperty.Cancel();
	}

TInt CVibraObserverAO::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}

//End of file
