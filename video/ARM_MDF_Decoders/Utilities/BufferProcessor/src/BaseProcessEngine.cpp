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

#include <devvideobase.h>
#include "BaseProcessEngine.h"
#include "RDebug.h"

/*
******************************************************************************
Name           : CBaseProcessEngine::CBaseProcessEngine
Description    : 1st phase construction
Parameter      : None
Return Value   : None
******************************************************************************
*/
CBaseProcessEngine::CBaseProcessEngine (TInt aPriority) :
	CActive ( aPriority ),
	iCodec ( NULL ),
	iObserver ( NULL ),
	iInput ( NULL ),
	iOutput ( NULL ),
    iState ( ENil ),
	iPrevResult ( MBaseCodec::EConsumed ),
	iInPlaceProcessing ( EFalse ),
	iClockSource(NULL),
	iTimeTakenForProcessing(0)
	{	
	}

/*
******************************************************************************
Name           : CBaseProcessEngine::NewL
Description    : 2phase construtor
Parameter      : aObserver - engine observer
				 aCodec - codec object
				 aInPlaceProcessing - 
Return Value   : constructed object
******************************************************************************
*/
CBaseProcessEngine* CBaseProcessEngine::NewL ( MProcessEngineObserver* aObserver,
											   MBaseCodec*	aCodec,
											   TBool aInPlaceProcessing,
                                               TInt aPriority)
	{
	CBaseProcessEngine* uSelf = new (ELeave) CBaseProcessEngine (aPriority);
	CleanupStack::PushL ( uSelf );
	uSelf->ConstructL ( aObserver, aCodec, aInPlaceProcessing );
	CleanupStack::Pop ();//uSelf
	return uSelf;
	}
 
/*
******************************************************************************
Name           : CBaseProcessEngine::~CBaseProcessEngine 
Description    : Destructor
Parameter      : None
Return Value   : None
******************************************************************************
*/
CBaseProcessEngine::~CBaseProcessEngine ()
	{
	Stop ();
	iCodec->DeInitialize ();
	iInputArray.Close ();
	iOutputArray.Close ();
	}

/*
******************************************************************************
Name           : CBaseProcessEngine::ConstructL
Description    : 2nd phase construtor
Parameter      : aObserver - engine observer
				 aCodec - codec object
				 aInPlaceProcessing - 
Return Value   : None
******************************************************************************
*/
void CBaseProcessEngine::ConstructL ( MProcessEngineObserver* aObserver,
									  MBaseCodec* aCodec,
									  TBool aInPlaceProcessing )
	{
	// add to the scheduler
	CActiveScheduler::Add ( this );

	//Set the observer
	iObserver			= aObserver;

	// Set the codec
	iCodec				= aCodec;

	//Set the inplace processing
	iInPlaceProcessing	= aInPlaceProcessing;

	// initialize the codec
	iCodec->InitializeL ();
	return;
	}

/*
******************************************************************************
Name           : CBaseProcessEngine::IssueRequest
Description    : To issue the request
Parameter      : None
Return Value   : None
******************************************************************************
*/
void CBaseProcessEngine::IssueRequest ()
	{
	if( iState != EStart )
		return;

	if ( IsActive () )
		{
		return;
		}
	TRequestStatus*  uStatus = &iStatus;
	User::RequestComplete ( uStatus, KErrNone );
	SetActive ();
	}

/*
******************************************************************************
Name           : CBaseProcessEngine::Start
Description    : Starts the engine
Parameter      : None
Return Value   : returns the error ( KErrNone if there is no error )
******************************************************************************
*/
TInt CBaseProcessEngine::Start ()
	{
	iState = EStart;
	IssueRequest ();
	return KErrNone;
	}




/*
******************************************************************************
Name           : CBaseProcessEngine::Stop
Description    : Stops the engine
Parameter      : None
Return Value   : returns the error ( KErrNone if there is no error )
******************************************************************************
*/
TInt CBaseProcessEngine::Stop ()
	{
	iState = EStop;
	Cancel ();
	return KErrNone;
	}

/*
******************************************************************************
Name           : CBaseProcessEngine::DoCancel
Description    : Function from CActive which will be called for canceling the object
Parameter      : None
Return Value   : None
******************************************************************************
*/
void CBaseProcessEngine::DoCancel ()
	{
	}

/*
******************************************************************************
Name           : CBaseProcessEngine::RunL
Description    : 
Parameter      : None
Return Value   : None
******************************************************************************
*/
void CBaseProcessEngine::RunL ()
	{
	RDP3(_L("CBaseProcessEngine::RunL ++"));
	if ( !IsReadyForProcessing () )
		{
		RDP3(_L("CBaseProcessEngine::RunL !IsReadyForProcessing () --"));
		return;
		}

	if (iInPlaceProcessing) 
		{
		//Skip Logic Goes Here
		// Check whether this can be postprocessed or skipped if the clock source is set
		if(iClockSource && (!CanProcess((TVideoPicture*)iInput)))
			{
			iObserver->InputBufferConsumed (FetchInputBuffer(), KErrUnderflow);
			}
        else
            {
		    iPrevResult = iCodec->DoProcessL (iInput);
		    switch ( iPrevResult )
			    {
			    case MBaseCodec::EConsumed:
			    case MBaseCodec::EInputConsumed:
				    iObserver->InputBufferConsumed (FetchInputBuffer(), KErrNone);
				    break;

			    case MBaseCodec::ENotConsumed:
			    case MBaseCodec::EOutputConsumed:
			    default:
				    break;
			    }
            }
		} 
	else 
		{
		//Skip Logic Goes Here
		// Check whether this can be postprocessed or skipped if the clock source is set
		if(iClockSource && (!CanProcess((TVideoPicture*)iInput)))
			{
			iObserver->InputBufferConsumed (FetchInputBuffer(), KErrUnderflow);
			}
        else
            {
		RDP3(_L("CBaseProcessEngine::DoProcessL ++"));
		iPrevResult = iCodec->DoProcessL ( iInput, iOutput );
		RDP3(_L("CBaseProcessEngine::DoProcessL --"));
		
		switch ( iPrevResult )
			{
			case MBaseCodec::EConsumed:
				RDP3(_L("InputBufferConsumed ++"));
				iObserver->InputBufferConsumed ( FetchInputBuffer(), KErrNone );
				RDP3(_L("InputBufferConsumed --"));
                RDP3(_L("OutputBufferReady ++"));
                iObserver->OutputBufferReady ( FetchOutputBuffer(), KErrNone );
                RDP3(_L("OutputBufferReady --"));
				break;
			case MBaseCodec::EInputConsumed:
				iObserver->InputBufferConsumed ( FetchInputBuffer(), KErrNone );
				break;
			case MBaseCodec::EOutputConsumed:
				iObserver->OutputBufferReady ( FetchOutputBuffer(), KErrNone );
				break;
			case MBaseCodec::ENotConsumed:
				break;
		    case MBaseCodec::EBadOutputBuffer:
		        iObserver->OutputBufferReady ( FetchOutputBuffer(), KErrCorrupt);
		        break;
		    case MBaseCodec::EBadInputBuffer:
		        break;
			default:
				break;
			    }
			}
		}

	IssueRequest ();
	RDP3(_L("CBaseProcessEngine::RunL --"));
	}

/*
******************************************************************************
Name           : CBaseProcessEngine::RunError
Description    : Handles when RunL() Leaves
Parameter      : None
Return Value   : None
******************************************************************************
*/
TInt CBaseProcessEngine::RunError ( TInt aError )
	{
	iObserver->HandleError ( aError );
	return KErrNone;
	}


/*
******************************************************************************
Name           : CBaseProcessEngine::GetInput
Description    : To get the input 
Parameter      : None
Return Value   : Input buffer
******************************************************************************
*/
TAny* CBaseProcessEngine::GetInput ()
	{
	if ( !iInputArray.Count () )
		{
		return NULL;
		}
	TAny* uBuffer = iInputArray[0];
	iInputArray.Remove ( 0 );
	return uBuffer;
	}


/*
******************************************************************************
Name           : CBaseProcessEngine::GetOutput
Description    : To get the output
Parameter      : None
Return Value   : Output object
******************************************************************************
*/
TAny* CBaseProcessEngine::GetOutput ()
	{
	RDP3(_L("CBaseProcessEngine::GetOutput--->"));
	if ( !iOutputArray.Count () )
		{
		return NULL;
		}
	TAny* uBuffer = iOutputArray[0];
	iOutputArray.Remove ( 0 );
	RDP1(_L("CBaseProcessEngine::GetOutput<--- %d"),uBuffer);
	return uBuffer;
	}

/*
******************************************************************************
Name           : CBaseProcessEngine::AddInput
Description    : Adds the input 
Parameter      : aInput - input
Return Value   : returns the error ( KErrNone if there is no error )
******************************************************************************
*/
TInt CBaseProcessEngine::AddInput ( TAny* aInput )
	{
		RDP3(_L("CBaseProcessEngine::AddInput"));
	if ( !aInput )
		{
		return KErrArgument;
		}
	if ( iInput == NULL )
		{
		iInput = aInput;
		if ( iState == EStart )
			{
			IssueRequest ();
			}
		return KErrNone;
		}
	return iInputArray.Append ( aInput );
	}

/*
******************************************************************************
Name           : CBaseProcessEngine::AddOutput
Description    : Adds the output 
Parameter      : aOutput -  output buffer
Return Value   : returns the error ( KErrNone if there is no error )
******************************************************************************
*/
TInt CBaseProcessEngine::AddOutput ( TAny* aOutput )
	{
	RDP1(_L("CBaseProcessEngine::AddOutput--> %d"),aOutput);
	if ( !aOutput )
		{
		return KErrArgument;
		}
	if ( iOutput == NULL)
		{
		iOutput = aOutput;
		RDP1(_L("CBaseProcessEngine::AddOutput %d"),iOutput);
		if ( iState == EStart )
			{
			IssueRequest ();
			}
		return KErrNone;
		}
	return iOutputArray.Append ( aOutput );
	}


/*
******************************************************************************
Name           : CBaseProcessEngine::IsReadyForProcessing
Description    : Checks for processing
Parameter      : None
Return Value   : None
******************************************************************************
*/
TBool CBaseProcessEngine::IsReadyForProcessing ()
	{ 
		
	if (iInPlaceProcessing) 
		{
		return (iInput != NULL) ? ETrue : EFalse;
		}
	return ((iInput != NULL) && (iOutput != NULL)) ? ETrue : EFalse; 

	}



/*
******************************************************************************
Name           : CBaseProcessEngine::Reset
Description    : Flush Output buffers
Parameter      : None
Return Value   : returns the error ( KErrNone if there is no error )
******************************************************************************
*/

		
TInt CBaseProcessEngine::Reset()
    {
	iCodec->Reset ();

	if(!iInput)
		iInput = GetInput ();

	while (iInput)
		iObserver->InputBufferConsumed (FetchInputBuffer(), KErrCancel);

	if(!iOutput)
		iOutput = GetOutput ();

	while (iOutput)
		iObserver->OutputBufferReady (FetchOutputBuffer(), KErrCancel);


	// reset the input array
	iInputArray.Reset ();
	// reset the output array
	iOutputArray.Reset ();

	return KErrNone;
    }

/*
******************************************************************************
Name           : NumInputBuffers 
Description    : Returns the number of input buffers in process engine
Parameter      :
Return Value   :
******************************************************************************
*/
TInt CBaseProcessEngine::NumInputBuffers ()
    {
	RDP3(_L("CBaseProcessEngine::NumInputBuffers--> "));
	TInt lCount = 0;
	if (iInput)
	    {
		lCount = 1;
	    }
	RDP3(_L("CBaseProcessEngine::NumInputBuffers<-- "));
	return (iInputArray.Count() + lCount);
    }
    
/*
******************************************************************************
Name           : NumOutputBuffers 
Description    : Returns the number of output buffers in process engine
Parameter      :
Return Value   :
******************************************************************************
*/
TInt CBaseProcessEngine::NumOutputBuffers ()
    {
	TInt lCount = 0;
	if (iOutput)
	    {
		lCount = 1;
	    }
	return (iOutputArray.Count() + lCount);
    }

/*
******************************************************************************
Name           : SetSyncOptions 
Description    : Set the clock source and the minimum time take for processinf
Parameter      :
Return Value   :
******************************************************************************
*/
TInt CBaseProcessEngine::SetSyncOptions(TAny* aClockSource,TInt aTimeTakenForProcessing)
    {
	iClockSource	= (MMMFClockSource*)aClockSource;
	iTimeTakenForProcessing = aTimeTakenForProcessing;
	return KErrNone;
    }


/*
*********************************************************************************
Name			: CanProcess
Description		: Function to determine whether processing should be done or skipped 
				  for the current i/p buffer.
Parameters		: aPicture - The picture to be postprocessed.
Return Value	: Boolean indicating whether postprocessing has to be done or not
*********************************************************************************
*/

TBool CBaseProcessEngine::CanProcess(TVideoPicture *aPicture)
	{

	TTimeIntervalMicroSeconds lTimeDifference = 0;
		
	lTimeDifference = TTimeIntervalMicroSeconds(
							(aPicture->iTimestamp.Int64()												 
							- iClockSource->Time().Int64()));												 
	if(lTimeDifference.Int64() >= iTimeTakenForProcessing)
		{
		return ETrue;
		}	

	return EFalse;
	}
/*
*********************************************************************************
Name			: FetchOutputBuffer
Description		: fetches the OutputBuffer from the process Engine OutPut Queue
Parameters		: None
Return Value	: Output Picture
*********************************************************************************
*/
TAny* CBaseProcessEngine::FetchOutputBuffer()
	{
	RDP3(_L("CBaseProcessEngine::FetchOutputBuffer--->"));
	TAny* lTempOutput=NULL;
	lTempOutput=iOutput;		
	iOutput=GetOutput();
	RDP1(_L("CBaseProcessEngine::FetchOutputBuffer  address<--- %d "),lTempOutput); 
	return lTempOutput;
	}

/*
*********************************************************************************
Name			: FetchInputBuffer
Description		: fetches the InputBuffer from the process Engine InPut Queue
Parameters		: None
Return Value	: Input Picture
*********************************************************************************
*/
TAny* CBaseProcessEngine::FetchInputBuffer()
	{
	RDP3(_L("CBaseProcessEngine::FetchInputBuffer--->"));
	TAny* iTempInput=NULL;
	iTempInput=iInput;		
	iInput=GetInput();
	RDP1(_L("CBaseProcessEngine::FetchInputBuffer  address<--- %d "),iTempInput); 
	return iTempInput;
	}
