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
* Description: Global Audio Settings Test
*
*/



// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "GlobalASTestClass.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CGlobalASTestClass::Delete
// Delete here all resources allocated and opened from test methods.
// Called from destructor.
// -----------------------------------------------------------------------------
//
void CGlobalASTestClass::Delete()
	{
    iLog->Log(_L("Deleting test class..."));
	if(iPhoneProfileSettings)
		{
		delete iPhoneProfileSettings;
		iPhoneProfileSettings = NULL;
		}
	}

// -----------------------------------------------------------------------------
// CGlobalASTestClass::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CGlobalASTestClass::RunMethodL( CStifItemParser& aItem )
    {

    static TStifFunctionInfo const KFunctions[] =
        {
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function.

		ENTRY( "CreateGlobalAS", CGlobalASTestClass::CreateGlobalAS ),
		ENTRY( "DeleteGlobalAS", CGlobalASTestClass::DeleteGlobalAS ),
        ENTRY( "IsWarningTonesEnabled", CGlobalASTestClass::IsWarningTonesEnabled ),
        ENTRY( "IsMessageTonesEnabled", CGlobalASTestClass::IsMessageTonesEnabled ),
        ENTRY( "IsSilentProfileEnabled", CGlobalASTestClass::IsSilentProfileEnabled ),
        ENTRY( "IsVibraEnabled", CGlobalASTestClass::IsVibraEnabled ),
        ENTRY( "IsPublicSilenceEnabled", CGlobalASTestClass::IsPublicSilenceEnabled ),
        ENTRY( "RegisterAudioClientsListObserver", CGlobalASTestClass::RegisterAudioClientsListObserver ),
        ENTRY( "UnregisterAudioClientsListObserver", CGlobalASTestClass::UnregisterAudioClientsListObserver ),
        ENTRY( "GetAudioClientsList", CGlobalASTestClass::GetAudioClientsList ),
        ENTRY( "GetPausedClientsList", CGlobalASTestClass::GetPausedClientsList )
        };

    const TInt count = sizeof( KFunctions ) /
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CGlobalASTestClass::CreateGlobalAS
// -----------------------------------------------------------------------------
//
TInt CGlobalASTestClass::CreateGlobalAS( CStifItemParser& /*aItem*/ )
	{
	iLog->Log(_L("CGlobalASTestClass::CreateGlobalAS"));
	TInt status(KErrNone);
	if(iPhoneProfileSettings)
		{
		status = KErrAlreadyExists;
		}
	else
		{
		TRAP(status,iPhoneProfileSettings = CGlobalAudioSettings::NewL(*this));
		}
	return status;
	}

// -----------------------------------------------------------------------------
// CGlobalASTestClass::DeleteGlobalAS
// -----------------------------------------------------------------------------
//
TInt CGlobalASTestClass::DeleteGlobalAS( CStifItemParser& /*aItem*/ )
	{
	iLog->Log(_L("CGlobalASTestClass::DeleteGlobalAS"));
	TInt status(KErrNone);
	if(iPhoneProfileSettings)
		{
		delete iPhoneProfileSettings;
		iPhoneProfileSettings = NULL;
		}
	else
		{
		status = KErrNotFound;
		}
	return status;
	}

// -----------------------------------------------------------------------------
// CGlobalASTestClass::IsWarningTonesEnabled
// -----------------------------------------------------------------------------
//
TInt CGlobalASTestClass::IsWarningTonesEnabled( CStifItemParser& /*aItem*/ )
	{
	iLog->Log(_L("CGlobalASTestClass::IsWarningTonesEnabled"));
	TInt status(KErrNone);
	if(iPhoneProfileSettings)
		{
		TBool enable = iPhoneProfileSettings->IsWarningTonesEnabled();
		iLog->Log(_L("CGlobalASTestClass::IsWarningTonesEnabled [%d]"),enable);
		}
	else
		{
		status = KErrNotFound;
		}
	return status;
	}
// -----------------------------------------------------------------------------
// CGlobalASTestClass::IsMessageTonesEnabled
// -----------------------------------------------------------------------------
//
TInt CGlobalASTestClass::IsMessageTonesEnabled( CStifItemParser& /*aItem*/ )
	{
	iLog->Log(_L("CGlobalASTestClass::IsMessageTonesEnabled"));
	TInt status(KErrNone);
	if(iPhoneProfileSettings)
		{
		TBool enable = iPhoneProfileSettings->IsMessageTonesEnabled();
		iLog->Log(_L("CGlobalASTestClass::IsMessageTonesEnabled [%d]"),enable);
		}
	else
		{
		status = KErrNotFound;
		}
	return status;
	}

// -----------------------------------------------------------------------------
// CGlobalASTestClass::IsSilentProfileEnabled
// -----------------------------------------------------------------------------
//
TInt CGlobalASTestClass::IsSilentProfileEnabled( CStifItemParser& /*aItem*/ )
	{
	iLog->Log(_L("CGlobalASTestClass::IsSilentProfileEnabled"));
	TInt status(KErrNone);
	if(iPhoneProfileSettings)
		{
		TBool enable = iPhoneProfileSettings->IsSilentProfileEnabled();
		iLog->Log(_L("CGlobalASTestClass::IsSilentProfileEnabled [%d]"),enable);
		}
	else
		{
		status = KErrNotFound;
		}
	return status;
	}

// -----------------------------------------------------------------------------
// CGlobalASTestClass::IsVibraEnabled
// -----------------------------------------------------------------------------
//
TInt CGlobalASTestClass::IsVibraEnabled( CStifItemParser& /*aItem*/ )
	{
	iLog->Log(_L("CGlobalASTestClass::IsVibraEnabled"));
	TInt status(KErrNone);
	if(iPhoneProfileSettings)
		{
		TBool enable = iPhoneProfileSettings->IsVibraEnabled();
		iLog->Log(_L("CGlobalASTestClass::IsVibraEnabled [%d]"),enable);
		}
	else
		{
		status = KErrNotFound;
		}
	return status;
	}

// -----------------------------------------------------------------------------
// CGlobalASTestClass::IsPublicSilenceEnabled
// -----------------------------------------------------------------------------
//
TInt CGlobalASTestClass::IsPublicSilenceEnabled( CStifItemParser& /*aItem*/ )
	{
	iLog->Log(_L("CGlobalASTestClass::IsPublicSilenceEnabled"));
	TInt status(KErrNone);
	if(iPhoneProfileSettings)
		{
		TBool enable = iPhoneProfileSettings->IsPublicSilenceEnabled();
		iLog->Log(_L("CGlobalASTestClass::IsPublicSilenceEnabled [%d]"),enable);
		}
	else
		{
		status = KErrNotFound;
		}
	return status;
	}

// -----------------------------------------------------------------------------
// CGlobalASTestClass::RegisterAudioClientsListObserver
// -----------------------------------------------------------------------------
//
TInt CGlobalASTestClass::RegisterAudioClientsListObserver( CStifItemParser& /*aItem*/ )
	{
	iLog->Log(_L("CGlobalASTestClass::RegisterAudioClientsListObserver"));
	TInt status(KErrNone);
	if(iPhoneProfileSettings)
		{
		status = iPhoneProfileSettings->RegisterAudioClientsListObserver(*this);
		}
	else
		{
		status = KErrNotFound;
		}
	return status;
	}

// -----------------------------------------------------------------------------
// CGlobalASTestClass::UnregisterAudioClientsListObserver
// -----------------------------------------------------------------------------
//
TInt CGlobalASTestClass::UnregisterAudioClientsListObserver( CStifItemParser& /*aItem*/ )
	{
	iLog->Log(_L("CGlobalASTestClass::UnregisterAudioClientsListObserver"));
	TInt status(KErrNone);
	if(iPhoneProfileSettings)
		{
		status = iPhoneProfileSettings->UnregisterAudioClientsListObserver(*this);
		}
	else
		{
		status = KErrNotFound;
		}
	return status;
	}

// -----------------------------------------------------------------------------
// CGlobalASTestClass::GetAudioClientsList
// -----------------------------------------------------------------------------
//
TInt CGlobalASTestClass::GetAudioClientsList( CStifItemParser& /*aItem*/ )
	{
	iLog->Log(_L("CGlobalASTestClass::GetAudioClientsList"));
	TInt status(KErrNone);
	if(iPhoneProfileSettings)
		{
	    RArray<TProcessId> aPIDList;
	    status = iPhoneProfileSettings->GetAudioClientsList( CGlobalAudioSettings::EActiveAudioClients, aPIDList );
	    if ( status != KErrNone )
	        {
	        iLog->Log(_L("CGlobalASTestClass:DisplayAll : Error[%d] getting Audio Clients List"), status );
	        }
	    else
	        {
	        iLog->Log(_L("CGlobalASTestClass:DisplayAll : Begins"));
	        if ( aPIDList.Count() == 0 )
	            {
	            iLog->Log(_L("No active audio client"));
	            }
	        else
	            {
	            for ( TInt index =0; index < aPIDList.Count(); index++ )
	                {
	                iLog->Log(_L("List[%d]PID[0x%x]"), index, aPIDList[index].Id());
	                }
	            }
	        iLog->Log(_L("CGlobalASTestClass:DisplayAll : Ends"));
	        }
	    aPIDList.Close();
		}
	else
		{
		status = KErrNotFound;
		}
	return status;
	}

// -----------------------------------------------------------------------------
// CGlobalASTestClass::GetAudioClientsList
// -----------------------------------------------------------------------------
//
TInt CGlobalASTestClass::GetPausedClientsList( CStifItemParser& /*aItem*/ )
	{
	iLog->Log(_L("CGlobalASTestClass::GetPausedClientsList"));
	TInt status(KErrNone);
	if(iPhoneProfileSettings)
		{
	    RArray<TProcessId> aPIDList;
	    status = iPhoneProfileSettings->GetAudioClientsList( CGlobalAudioSettings::EPausedAudioClients, aPIDList );
	    if ( status != KErrNone )
	        {
	        iLog->Log(_L("CGlobalASTestClass:DisplayAll : Error[%d] getting Paused Clients List"), status );
	        }
	    else
	        {
	        iLog->Log(_L("CGlobalASTestClass:DisplayAll : Begins"));
	        if ( aPIDList.Count() == 0 )
	            {
	            iLog->Log(_L("No paused audio client"));
	            }
	        else
	            {
	            for ( TInt index =0; index < aPIDList.Count(); index++ )
	                {
	                iLog->Log(_L("List[%d]PID[0x%x]"), index, aPIDList[index].Id());
	                }
	            }
	        iLog->Log(_L("CGlobalASTestClass:DisplayAll : Ends"));
	        }
	    aPIDList.Close();
		}
	else
		{
		status = KErrNotFound;
		}
	return status;
	}



void CGlobalASTestClass::SettingsChanged(CGlobalAudioSettings& /*aGlobalAudioSettings*/, TUint /*aSetting*/)
	{
    iLog->Log(_L("CGlobalASTestClass:SettingsChanged"));
	}

void CGlobalASTestClass::Event( const CGlobalAudioSettings& /*aGlobalAudioSettings*/, TUint aEvent )
    {
    iLog->Log(_L("CGlobalASTestClass From MAudioClientsListObserver[%d]"), aEvent);
    }

//  End of File
