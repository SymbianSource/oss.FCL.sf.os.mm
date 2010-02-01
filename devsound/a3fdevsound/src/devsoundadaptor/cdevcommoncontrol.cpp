// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "cdevcommoncontrol.h"
#include <a3f/maudiocontext.h> 
#include <a3f/maudiocodec.h>


CDevCommonControl::CDevCommonControl()
    {
    TRACE_CREATE();
    DP_CONTEXT(CDevCommonControl::CDevCommonControl, CtxDevSound, DPLOCAL);
    DP_IN();
    DP_OUT();
    }


CDevCommonControl::~CDevCommonControl()
    {
    DP_CONTEXT(CDevCommonControl::~CDevCommonControl, CtxDevSound, DPLOCAL);
    DP_IN();
    DP_OUT();
    }


TInt CDevCommonControl::Stop() // from CDevAudioControl
    {
    DP_CONTEXT(CDevCommonControl::Stop, CtxDevSound, DPLOCAL);
    DP_IN();

    TInt err = KErrNone;
    switch(iDevAudio->iActiveState)
        {
        case EDevSoundAdaptorActive_Active:
        case EDevSoundAdaptorPaused_Primed:
            err = iDevAudio->iAudioStream->Stop();
            if (err == KErrNone)
                {
                err = iDevAudio->iAudioContext->Commit();
                }
            if (err == KErrNone)
                {
                iDevAudio->iActiveState = EDevSoundAdaptorStopping;
                }
            break;
        case EDevSoundAdaptorGoingActive:
            iDevAudio->iActiveState = EDevSoundAdaptorStopping;
            break;
        default:
            break;
        }
    
    if(err == KErrNone)
        {
        iDevAudio->iStop = ETrue;
        }
    
    DP0_RET(err,"%d");
    }


TInt CDevCommonControl::Pause() // from CDevAudioControl
    {
    DP_CONTEXT(CDevCommonControl::Pause, CtxDevSound, DPLOCAL);
    DP_IN();

    TInt err = iDevAudio->iAudioStream->Prime();
    if ( err == KErrNone)
        {
        err = iDevAudio->iAudioContext->Commit();
        }
    if (err == KErrNone)
        {
        iDevAudio->iActiveState = EDevSoundAdaptorPausing;
        }
    
    DP0_RET(err,"%d");
    }


TInt CDevCommonControl::Resume() // from CDevAudioControl
    {
    DP_CONTEXT(CDevCommonControl::Resume, CtxDevSound, DPLOCAL);
    DP_IN();
    
    TInt err = KErrNone;
    if(iDevAudio->iActiveState != EDevSoundAdaptorPaused_Primed)
        {
        err = KErrNotReady;
        }

    if(err == KErrNone)
        {
        err = iDevAudio->iAudioStream->Activate();
        }
    if ( err == KErrNone)
        {
        err = iDevAudio->iAudioContext->Commit();
        }
    if (err == KErrNone)
        {
        iDevAudio->iActiveState = EDevSoundAdaptorActivating;
        }
    
    DP0_RET(err,"%d");
    }


void CDevCommonControl::StateEvent(MAudioStream& aStream, TInt aError,  // from MAudioProcessingUnitObserver
                                    TAudioState aNewState)
    {
    DP_CONTEXT(CDevCommonControl::StateEvent, CtxDevSound, DPLOCAL);
    DP3_IN("activeState %d -> newstate %d, (%d)",
            iDevAudio->iActiveState, aNewState, aError);
    
    __ASSERT_ALWAYS(iDevAudio->iAudioStream == &aStream, Panic(EStreamMismatch));
    
    if(aError != KErrNone || iDevAudio->iActiveStreamState != aNewState) 
        {
        iDevAudio->iActiveStreamState = aNewState;
        iStateEventReceived = ETrue;
        }
    // Since the audiostream already demoted the state for the most of the cases
    // only is need  when a error comes were the stream was at the middle of A3f two-phase transition
    else
        {
        switch (aNewState)
            {
            case EIdle:
                // Demote the stream state
                iDevAudio->iActiveStreamState = EIdle;
                break;
            default:
                break;
            }
        }
    iStateEventError = aError;
    
    DP_OUT();
    }


void CDevCommonControl::ProcessingUnitError(MAudioProcessingUnit* /*aInstance*/, // from MAudioProcessingUnitObserver
                                                                 TInt aError)
    {
    DP_CONTEXT(CDevCommonControl::ProcessingUnitError, CtxDevSound, DPLOCAL);
    DP_IN();

    if(iCallbackFromAdaptor == KCallbackNone)   
        {
        iCallbackFromAdaptor = KCallbackProcessingUnitError;
        iAdaptationObserver->CallbackFromAdaptorReceived(KCallbackProcessingUnitError, KErrNone);
        iProcessingUnitError = aError;
        }
    else
        {
        // Multiple callbacks from adaptor
        DP0(DLINFO, "Multiple callbacks from adaptor");
        }
    
    DP_OUT();
    }


void CDevCommonControl::ContextEvent(TUid aEvent, TInt aError) 
    {
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();

    // Can't "switch {...}" on UIDs unfortunately:

    if (aEvent == KUidA3FContextUpdateComplete)
        {
        //use a sub state pattern to make premtion cycles like other cycles.
        if(iBeingPreempted)
            {
            DP1(DLERR,"Preemption error=%d", aError);
            CDevAudioControl::ContextEvent(aEvent, aError);
            iDevAudio->iActiveState = EDevSoundAdaptorBeingPreempted;
            iBeingPreempted=EFalse;
            }
        ContextEventUpdateComplete(aError);
        }

    else if ((aEvent == KUidA3FContextPreEmption) or (aEvent == KUidA3FContextPreEmptedCommit))
        {
        //we are not being preemptied
        iBeingPreempted=EFalse;
        ContextEventPreEmption(aEvent, aError);
        }

    else if (aEvent == KUidA3FContextAbort)
        {
        ContextEventAbort(aError);
        }

    DP_OUT();
    }


void CDevCommonControl::ContextEventAsynchronousPlayCompletion(TInt aError) // from CDevCommonControl
    {
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();
    
	iAdaptationObserver->AsynchronousOperationComplete(aError, ETrue);
		
    if (aError)
        {
        FinishWithError(aError);
        }
           
    DP_OUT();
    }


void CDevCommonControl::ContextEventAsynchronousInitializeComplete(TInt aError) // from CDevCommonControl
    {
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();
    
    iAdaptationObserver->AsynchronousOperationComplete(aError, ETrue);
    iAdaptationObserver->InitializeComplete(aError);
    
    DP_OUT();
    }


void CDevCommonControl::ContextEventUpdateComplete(TInt aError) // from CDevCommonControl
    {
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();
    
    if (iStateEventReceived)
        {
        iStateEventReceived = EFalse;
        DP0(DLINFO,"Context event for that does involve state change");         
        
        if (aError)
            {
            ContextEventUpdateWithStateEventAndError(aError);
            }
        else
            {
            ContextEventUpdateWithStateEventNoError();
            }   
        DP_OUT();
        return;
        }
    
    DP0(DLINFO,"Context event for that doesn't involve state change");

    if (aError)
        {
        ContextEventUpdateWithoutStateEventButWithError(aError);
        }
    else
        {
        ContextEventUpdateWithoutStateEventNoError();
        }
    
    DP_OUT();
    }


void CDevCommonControl::ContextEventPreEmption(TUid aEvent, TInt aError) // from CDevCommonControl
    {
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();
    
    DP1(DLERR,"Preemption error=%d", aError);
    CDevAudioControl::ContextEvent(aEvent, aError);
    iBeingPreempted=ETrue;
        
    DP_OUT();
    }


void CDevCommonControl::ContextEventAbort(TInt aError) // from CDevCommonControl
    {
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();
    
    DP1(DLERR,"Abort error=%d", aError);
    FinishWithError(aError==KErrAbort ? KErrDied:aError);
    
    DP_OUT();
    }


void CDevCommonControl::ContextEventStopDevSoundNotifications() // from CDevCommonControl
    {
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();
    
    iDevAudio->iAudioStream->UnregisterAudioStreamObserver(*this);
    iGainControl->UnregisterAudioGainControlObserver(*this);
    iAudioCodecIf->UnregisterAudioCodecObserver(*this);
    iAudioCodecIf = NULL;
    
    DP_OUT();
    }


void CDevCommonControl::ContextEventPauseResumeSequenceDueToEmptyBuffers(TBool aFlush) // from CDevCommonControl
    {
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();
    
    TInt err(KErrNone);

    if (iPauseResumeSequenceDueToEmptyBuffers)
        {
        if (aFlush)
            {
            err = iDevAudio->iAudioStream->Flush();
            }
        
        if ((err) or (aFlush==EFalse))
            {
            iPauseResumeSequenceDueToEmptyBuffers = EFalse;
            iAdaptationObserver->CallbackFromAdaptorReceived(KCallbackFlushComplete, err);
            }
        }
    else
        {
        iAdaptationObserver->AsynchronousOperationComplete(KErrNone, ETrue);    
        }
    
    DP_OUT();
    }


void CDevCommonControl::ContextEventStateDevSoundAdaptorUnloading() // from CDevCommonControl
    {
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();
    
    // Due destruction sequence or reinitialization
    if (iDevAudio->iClosing or iDevAudio->iReinitializing)
        {
        TInt err = Uninitialize();
        if (err and iDevAudio->iReinitializing)
            {
            ContextEventAsynchronousInitializeComplete(err);
    
            }
        
        DP_OUT();
        return;
        }

    // Notify the user that ProcessingFinished is complete. 
    // Stop call complete, sent callback.
    if (iCallbackFromAdaptor != KCallbackNone)
        {
        iAdaptationObserver->AsynchronousOperationComplete(KErrNone, ETrue);
        
        if (iCallbackFromAdaptor == KCallbackProcessingFinished)
            {
            FinishWithError(KErrUnderflow);
            }
        else if (iCallbackFromAdaptor == KCallbackProcessingUnitError)
            {
            FinishWithError(iProcessingUnitError);
            }
        
        iCallbackFromAdaptor = KCallbackNone;
        DP_OUT();
        return;
        }
    
    // Error condition
    if (iErrorCondition)
        {
        FinishWithError(iErrorCondition);
        iErrorCondition = KErrNone;
        }
    else
        {
    
        iAdaptationObserver->AsynchronousOperationComplete(KErrNone, ETrue);
        }
    
    DP_OUT();
    }


void CDevCommonControl::ContextEventStateDevSoundAdaptorLoading() // from CDevCommonControl
    {
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();
    
    iDevAudio->RequestGainAndBalance(this); // TODO handle error

    TInt err = iDevAudio->iAudioStream->Activate();
    if (err)
        {
        DP_OUT();
        return;
        }
    
    err = iDevAudio->iAudioContext->Commit();
    if (err)
        {
        ContextEventAsynchronousPlayCompletion(err);
        DP_OUT();
        return;
        }
    
    iDevAudio->iActiveState = EDevSoundAdaptorActivating;
    iAdaptationObserver->AsynchronousOperationComplete(KErrNone, EFalse);
    
    DP_OUT();
    }


void CDevCommonControl::ContextEventStateDevSoundAdaptorStopping() // from CDevCommonControl
    {   
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();
    
    TInt err = Unload();
    if (err)
        {
        DP0(DLINFO,"Commit failed during stopping");
        FinishWithError(err);
        }
    
    __ASSERT_DEBUG(err==KErrNone, Panic(ECommitFailedDuringStop));
    
    DP_OUT();
    }


void CDevCommonControl::ContextEventStateDevSoundAdaptorBeingPreempted() // from CDevCommonControl
    {
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();
    
    __ASSERT_DEBUG(iDevAudio->iActiveStreamState == EInitialized, Panic(EStreamBeingDemotedToEIdle));
    FinishWithError(KErrInUse);
    if (iIgnoreAsyncOpComplete)
        {
        iAdaptationObserver->PreemptionFinishedCallbackReceived(ETrue);
        iIgnoreAsyncOpComplete=EFalse;
        DP_OUT();
        return;
        }
    
    ContextEventPauseResumeSequenceDueToEmptyBuffers(EFalse);
    
    DP_OUT();
    }


void CDevCommonControl::ContextEventStateDevSoundAdaptorUninitializing() // from CDevCommonControl
    {
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();
    TInt err = RemoveProcessingUnits();

    if (err == KErrNone)
        {
        iDevAudio->iActiveState = EDevSoundAdaptorRemovingProcessingUnits;
        }
    else if (iDevAudio->iReinitializing)
        {
        ContextEventAsynchronousInitializeComplete(err);
        }
    
    DP_OUT();
    }


void CDevCommonControl::ContextEventErrorStateDevSoundAdaptorActivating(TInt aError) // from CDevCommonControl
    {
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();

    // If the resume operation fails as result of EmptyBuffers
    // Notify about operation complete through CallbackFromAdaptorReceived
    // and continue to allow client to receive PlayError()
    if (iPauseResumeSequenceDueToEmptyBuffers)
        {
        iPauseResumeSequenceDueToEmptyBuffers = EFalse;
        iAdaptationObserver->CallbackFromAdaptorReceived(KCallbackFlushComplete, KErrNone);
        }
    
    iErrorCondition = aError;
    
    TInt err = Unload();
    if (err)
        {
        DP0(DLINFO,"Commit failed during stopping");
        FinishWithError(err);
        }
    __ASSERT_DEBUG(err==KErrNone, Panic(ECommitFailedDuringStop));
    
    DP_OUT();
    }


void CDevCommonControl::ContextEventErrorStateDevSoundAdaptorBeingPreempted() // from CDevCommonControl
    {
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();
    
__ASSERT_DEBUG(iDevAudio->iActiveStreamState == EInitialized, Panic(EStreamBeingDemotedToEIdle));
    FinishWithError(KErrInUse);

    if(iIgnoreAsyncOpComplete)
        {
        iAdaptationObserver->PreemptionFinishedCallbackReceived(ETrue);
        }
    
    DP_OUT();
    }


void CDevCommonControl::ContextEventUpdateWithoutStateEventNoError() // from CDevCommonControl
    {
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();
    
    if (iDevAudio->iActiveState != EDevSoundAdaptorRemovingProcessingUnits)
        {
        iAdaptationObserver->AsynchronousOperationComplete(KErrNone, ETrue);
        DP_OUT();
        return;
        }

    iDevAudio->iActiveState = EDevSoundAdaptorCreated_Uninitialised;
    
    if (iDevAudio->iReinitializing)
        {
        ContextEventStopDevSoundNotifications();
        
        TInt err = iDevAudio->Initialize(iDevAudio->iTargetFormat, iDevAudio->iTargetMode);
        if(err)
            {
            ContextEventAsynchronousInitializeComplete(err);
            }

        iDevAudio->iReinitializing = EFalse;
        DP_OUT();
        return;
        }
    
    iDevAudio->iClosing = EFalse;
    iAdaptationObserver->AsynchronousOperationComplete(KErrNone, ETrue);
    
    DP_OUT();
    }


void CDevCommonControl::ContextEventUpdateWithoutStateEventButWithError(TInt aError) // from CDevCommonControl
    {
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();
    
	// NOTE: If no state change then do NOT complete the event.
	
    // NOTE: We shouldn't actually be in any of the states below when calling this function.
    //       But just in case we are we will rewind the state before dealing with the error. 
    switch (iDevAudio->iActiveState)
        {
    case EDevSoundAdaptorInitialising:
        iDevAudio->iActiveState = EDevSoundAdaptorCreated_Uninitialised;
        ContextEventAsynchronousInitializeComplete(aError);
        break;
        
    case EDevSoundAdaptorLoading:
        iDevAudio->iActiveState = EDevSoundAdaptorInitialised_Initialised;
        ContextEventAsynchronousPlayCompletion(aError);
        break;
        
    case EDevSoundAdaptorActivating:
        iDevAudio->iActiveState = EDevSoundAdaptorInitialised_Idle;
        ContextEventAsynchronousPlayCompletion(aError);
        break;
        
    default:
        break;
        }
       
    DP_OUT();
    }


void CDevCommonControl::ContextEventUpdateWithStateEventNoError() // from CDevCommonControl 
    {
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();

    switch (iDevAudio->iActiveState)
        {
    case EDevSoundAdaptorUninitialising:
        iDevAudio->iActiveState = EDevSoundAdaptorUnitialised_Uninitialised;
        ContextEventStateDevSoundAdaptorUninitializing();
        break;

    case EDevSoundAdaptorInitialising:
        iDevAudio->iActiveState = EDevSoundAdaptorInitialised_Initialised;
        ContextEventAsynchronousInitializeComplete(KErrNone);
        break;

    case EDevSoundAdaptorUnloading:
        iDevAudio->iActiveState = EDevSoundAdaptorInitialised_Initialised;
        ContextEventStateDevSoundAdaptorUnloading();
        break;

    case EDevSoundAdaptorLoading:
        iDevAudio->iActiveState = EDevSoundAdaptorGoingActive;
        ContextEventStateDevSoundAdaptorLoading();
        break;

    case EDevSoundAdaptorStopping:
        iDevAudio->iActiveState = EDevSoundAdaptorInitialised_Idle;
        ContextEventStateDevSoundAdaptorStopping();
        break;

    case EDevSoundAdaptorActivating:
        iDevAudio->iActiveState = EDevSoundAdaptorActive_Active;
        ContextEventPauseResumeSequenceDueToEmptyBuffers(EFalse);
        break;
        
    case EDevSoundAdaptorPausing:
        iDevAudio->iActiveState = EDevSoundAdaptorPaused_Primed;
        ContextEventPauseResumeSequenceDueToEmptyBuffers(ETrue);
        break;

    case EDevSoundAdaptorBeingPreempted:
        iDevAudio->iActiveState = EDevSoundAdaptorInitialised_Initialised;
        ContextEventStateDevSoundAdaptorBeingPreempted();
        break;
        
    default:
        break;
        }   
    
    DP_OUT();
    }


void CDevCommonControl::ContextEventUpdateWithStateEventAndError(TInt aError) // from CDevCommonControl
    {
    DP_CONTEXT(CDevCommonControl::ContextEvent, CtxDevSound, DPLOCAL);
    DP_IN();

    DP1(DLERR,"ContextEvent error=%d", aError);
    
    switch(iDevAudio->iActiveState)
        {
    case EDevSoundAdaptorInitialising:
        iDevAudio->iActiveState = EDevSoundAdaptorCreated_Uninitialised;
        iAdaptationObserver->InitializeComplete(aError);
        break;
        
    case EDevSoundAdaptorLoading:
        iDevAudio->iActiveState = EDevSoundAdaptorInitialised_Initialised;
        FinishWithError(aError);
        break;
        
    case EDevSoundAdaptorActivating:
        iDevAudio->iActiveState = EDevSoundAdaptorInitialised_Idle;
        ContextEventErrorStateDevSoundAdaptorActivating(aError);
        break;
        
    case EDevSoundAdaptorBeingPreempted:
        iDevAudio->iActiveState = EDevSoundAdaptorInitialised_Initialised;
        ContextEventErrorStateDevSoundAdaptorBeingPreempted();
        break;

    default:
        break;
        }
    
    iCallbackFromAdaptor = KCallbackNone;
    
    if(iIgnoreAsyncOpComplete==EFalse)
        {
        iAdaptationObserver->AsynchronousOperationComplete(aError, ETrue);
        }
    
    iIgnoreAsyncOpComplete=EFalse;
    
    DP_OUT();
    }


// end of file
