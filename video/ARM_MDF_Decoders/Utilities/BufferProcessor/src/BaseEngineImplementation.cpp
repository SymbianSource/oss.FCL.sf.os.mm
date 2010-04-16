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
* Description:  
*
*/

#include "BaseEngine.h"
#include "BaseEngineImplementation.h"
#include "BaseProcessEngine.h"

/*
******************************************************************************
Name           : BaseEngine::NewL
Description    : 2phase construtor
Parameter      : aObserver - engine observer
				 aCodec - codec object
				 aInPlaceProcessing - 
Return Value   : constructed object
******************************************************************************
*/
EXPORT_C CBaseEngine* CBaseEngine::NewL ( MProcessEngineObserver* aObserver,
										  MBaseCodec* aCodec,
										  TBool aInPlaceProcessing,
                                          TInt aPriority)
	{
	return CBaseEngineImplementation::NewL ( aObserver, aCodec, aInPlaceProcessing, aPriority );
	}	
/*
******************************************************************************
Name           : CBaseEngineImplementation::NewL
Description    : 2phase construtor
Parameter      : aObserver - engine observer
				 aCodec - codec object
				 aInPlaceProcessing - 
Return Value   : constructed object
******************************************************************************
*/
CBaseEngineImplementation* CBaseEngineImplementation::NewL ( MProcessEngineObserver* aObserver,
														     MBaseCodec* aCodec,
														     TBool aInPlaceProcessing,
                                                             TInt aPriority)
	{
	CBaseEngineImplementation* uSelf = new (ELeave) CBaseEngineImplementation;
	CleanupStack::PushL ( uSelf );
	uSelf->ConstructL ( aObserver, aCodec, aInPlaceProcessing, aPriority );
	CleanupStack::Pop (); //uSelf
	return uSelf;
	}

/*
******************************************************************************
Name           : CBaseEngineImplementation::CBaseEngineImplementation 
Description    : 1st phase constructor
Parameter      : None
Return Value   : None
******************************************************************************
*/
CBaseEngineImplementation::CBaseEngineImplementation ():
	iEngineHandle ( NULL )
	{
	}

/*
******************************************************************************
Name           : CBaseEngineImplementation::~CBaseEngineImplementation
Description    : Destructor
Parameter      : None
Return Value   : None
******************************************************************************
*/
CBaseEngineImplementation::~CBaseEngineImplementation ()
	{
	if ( iEngineHandle )
		{
		delete iEngineHandle;
		iEngineHandle = 0;
		}
	}

/*
******************************************************************************
Name           : CBaseEngineImplementation::ConstructL
Description    : 2nd phase construtor
Parameter      : aObserver - engine observer
				 aCodec - codec object
				 aInPlaceProcessing - 
Return Value   : None
******************************************************************************
*/
void CBaseEngineImplementation::ConstructL ( MProcessEngineObserver* aObserver,
											 MBaseCodec* aCodec,
											 TBool aInPlaceProcessing,
                                             TInt aPriority)
	{
	iEngineHandle = CBaseProcessEngine::NewL ( aObserver, aCodec, aInPlaceProcessing, aPriority );
	}

/*
******************************************************************************
Name           : CBaseEngineImplementation::AddInput
Description    : Adds the input 
Parameter      : aInput - input
Return Value   : returns the error ( KErrNone if there is no error )
******************************************************************************
*/
TInt CBaseEngineImplementation::AddInput ( TAny* aInput )
	{
	if ( !iEngineHandle )
		{
		return KErrNotReady;
		}
	return iEngineHandle->AddInput ( aInput );
	}

/*
******************************************************************************
Name           : CBaseEngineImplementation::AddOutput
Description    : Adds the output 
Parameter      : aOutput -  output buffer
Return Value   : returns the error ( KErrNone if there is no error )
******************************************************************************
*/
TInt CBaseEngineImplementation::AddOutput ( TAny* aOutput )
	{
	if ( !iEngineHandle )
		{
		return KErrNotReady;
		}
	return iEngineHandle->AddOutput ( aOutput );
	}

/*
******************************************************************************
Name           : CBaseEngineImplementation::Start
Description    : Starts the engine
Parameter      : None
Return Value   : returns the error ( KErrNone if there is no error )
******************************************************************************
*/
TInt CBaseEngineImplementation::Start ()
	{
	if ( !iEngineHandle )
		{
		return KErrNotReady;
		}
	return iEngineHandle->Start ();
	}

/*
******************************************************************************
Name           : CBaseEngineImplementation::Stop
Description    : Stops the engine
Parameter      : None
Return Value   : returns the error ( KErrNone if there is no error )
******************************************************************************
*/
TInt CBaseEngineImplementation::Stop ()
	{
	if ( !iEngineHandle )
		{
		return KErrNotReady;
		}
	return iEngineHandle->Stop ();
	}



/*
******************************************************************************
Name           : CBaseEngineImplementation::Reset
Description    : Reset all the buffers.
Parameter      : None
Return Value   : returns the error ( KErrNone if there is no error )
******************************************************************************
*/
TInt CBaseEngineImplementation::Reset()
    {
	if ( !iEngineHandle )
		{
		return KErrNotReady;
		}
	return iEngineHandle->Reset ();
    }

/*
******************************************************************************
Name           : NumInputBuffers 
Description    : Returns the number of input buffers in process engine
Parameter      :
Return Value   :
******************************************************************************
*/
TInt CBaseEngineImplementation::NumInputBuffers ()
    {
	if ( !iEngineHandle )
	    {
		return 0;
	    }
	return iEngineHandle->NumInputBuffers ();
    }

/*
******************************************************************************
Name           : NumOutputBuffers 
Description    : Returns the number of output buffers in process engine
Parameter      :
Return Value   :
******************************************************************************
*/
TInt CBaseEngineImplementation::NumOutputBuffers ()
    {
	if ( !iEngineHandle )
	    {
		return 0;
	    }
	return iEngineHandle->NumOutputBuffers ();
    }



/*
******************************************************************************
Name           : SetSyncOptions 
Description    : Set the synchronization options 
Parameter      : aClockSource	 - clock sourec to use for syncronization
				 aTimeTakenForProcessing - Time taken for Processing
Return Value   :
******************************************************************************
*/
TInt CBaseEngineImplementation::SetSyncOptions(TAny *aClockSource, TInt  aTimeTakenForProcessing)
    {
	if ( !iEngineHandle )
	    {
		return 0;
	    }
	return iEngineHandle->SetSyncOptions(aClockSource, aTimeTakenForProcessing);
    }

/*
******************************************************************************
Name           : FetchOutputBuffers 
Description    : fetches the OutputBuffer from the process Engine OutPut Queue
Parameter      : None
Return Value   : output buffer
******************************************************************************
*/
TAny* CBaseEngineImplementation::FetchOutputBuffer() 
	{
	if ( !iEngineHandle )
	    {
		return 0;
	    }

	return iEngineHandle->FetchOutputBuffer();
	}


/*
******************************************************************************
Name           : FetchInputBuffer 
Description    : fetches the inputBuffer from the process Engine inPut Queue
Parameter      : None
Return Value   : inputBuffer
******************************************************************************
*/
TAny* CBaseEngineImplementation::FetchInputBuffer() 
	{
	if ( !iEngineHandle )
	    {
		return 0;
	    }
	
	return iEngineHandle->FetchInputBuffer();
	}
	
	