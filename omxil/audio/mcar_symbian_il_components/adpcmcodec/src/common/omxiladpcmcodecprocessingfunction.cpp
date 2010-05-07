/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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


/**
   @file
   @internalComponent
*/

#include "omxiladpcmcodecprocessingfunction.h"
#include "OmxILAdPcmCodecConsts.h"
#include "omxilsymbianadpcmdecoderextensions.h"
#include "omxilcallbacknotificationif.h"
#include "log.h"

COmxILAdPcmCodecProcessingFunction* COmxILAdPcmCodecProcessingFunction::NewL(COmxILAdPcmCodecProcessingFunctionCore* aCore, MOmxILCallbackNotificationIf& aCallbacks)
	{
	COmxILAdPcmCodecProcessingFunction* self = new(ELeave)COmxILAdPcmCodecProcessingFunction(aCore, aCallbacks);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);

	return self;
	}

COmxILAdPcmCodecProcessingFunction::COmxILAdPcmCodecProcessingFunction(COmxILAdPcmCodecProcessingFunctionCore* aCore, MOmxILCallbackNotificationIf& aCallbacks)
	: COmxILProcessingFunction(aCallbacks)
	, iCore( aCore )
	{
	iCore->SetObserver( this );
	}

void COmxILAdPcmCodecProcessingFunction::ConstructL()
	{
	
	}

COmxILAdPcmCodecProcessingFunction::~COmxILAdPcmCodecProcessingFunction()
	{
	//we do not own iCore;

	iBuffersToEmpty.Reset();//we don't own the buffers
	iBuffersToFill.Reset();//we don't own the buffers
	}


OMX_ERRORTYPE COmxILAdPcmCodecProcessingFunction::StateTransitionIndication(COmxILFsm::TStateIndex aNewState)
	{
	OMX_ERRORTYPE err = OMX_ErrorNone;

	switch(aNewState)
		{
		case COmxILFsm::EStateIdle:
			{
			iCore->Reset();
			//
			TInt symerr = iCore->CheckConversion();
			if (symerr == KErrNone )
				{
				symerr = iCore->Configure();
				}
			if (symerr == KErrNotSupported)
				{
				err = OMX_ErrorUnsupportedSetting;
				}
			else if(symerr!=KErrNone) 
				{
				err = OMX_ErrorUndefined;
				}
			
			}
			break;

		case COmxILFsm::EStateExecuting:
			iCore->Start();
			break;

		case COmxILFsm::EStateInvalid:
		case COmxILFsm::EStateLoaded:
		case COmxILFsm::EStateWaitForResources:
			iCore->Reset();
			break;

		case COmxILFsm::EStatePause:
			iCore->Stop();
			break;

		case COmxILFsm::ESubStateLoadedToIdle:
		case COmxILFsm::ESubStateIdleToLoaded://Deinitalization
		case COmxILFsm::ESubStateExecutingToIdle:
		case COmxILFsm::ESubStatePauseToIdle:
			// do nothing
			break;
		}

	iState = aNewState;
	return err;
	}

OMX_ERRORTYPE COmxILAdPcmCodecProcessingFunction::BufferFlushingIndication(TUint32 aPortIndex, OMX_DIRTYPE aDirection)
	{
	TBool portflushed = EFalse;

	// flush the input buffers if requested
	if ((aPortIndex == OMX_ALL && aDirection == OMX_DirMax) ||
			(aPortIndex == 0 && aDirection == OMX_DirInput))
		{
		// reset the core to release any in use buffers
		iCore->Reset();

		// send notification for each input buffer
		for (TInt i=0; i<iBuffersToEmpty.Count(); i++)
			{
			iCallbacks.BufferDoneNotification(iBuffersToEmpty[i], 0, OMX_DirInput);
			}

		// release the buffers
		iBuffersToEmpty.Reset();

		portflushed = ETrue;
		}

	// flush the output buffers if requested
	if ((aPortIndex == OMX_ALL && aDirection == OMX_DirMax) ||
			(aPortIndex == 1 && aDirection == OMX_DirOutput))
		{
		// reset the core to release any in use buffers
		iCore->Reset();

		// send notification for each input buffer
		for (TInt i=0; i<iBuffersToFill.Count(); i++)
			{
			iCallbacks.BufferDoneNotification(iBuffersToFill[i], 1, OMX_DirOutput);
			}

		// release the buffers
		iBuffersToFill.Reset();

		portflushed = ETrue;
		}

	// Check we flushed any port
	if (!portflushed)
		return OMX_ErrorUndefined;

	return OMX_ErrorNone;
	}

OMX_ERRORTYPE COmxILAdPcmCodecProcessingFunction::ParamIndication(OMX_INDEXTYPE aParamIndex, const TAny* apComponentParameterStructure)
	{
	OMX_ERRORTYPE err = OMX_ErrorNone;

	switch(aParamIndex)
        {
        case OMX_SymbianIndexParamAudioAdPcmDecoderBlockAlign:
            {
            const OMX_SYMBIAN_AUDIO_PARAM_ADPCMDECODER_BLOCKALIGN* pAdPcmDecoderParam
                = static_cast<const OMX_SYMBIAN_AUDIO_PARAM_ADPCMDECODER_BLOCKALIGN*>(
                apComponentParameterStructure);

            err = iCore->SetBlockAlign(pAdPcmDecoderParam->nBlockAlign);
            }
            break;

        default:
            // ignore
            break;
        }

    return err;
	}

OMX_ERRORTYPE COmxILAdPcmCodecProcessingFunction::ConfigIndication(OMX_INDEXTYPE /*aConfigIndex*/, const TAny* /*apComponentConfigStructure*/)
	{
    // no configs are processable for this codec...
    return OMX_ErrorUnsupportedSetting;

	}

OMX_ERRORTYPE COmxILAdPcmCodecProcessingFunction::BufferIndication(OMX_BUFFERHEADERTYPE* apBufferHeader, OMX_DIRTYPE aDirection)
	{
	if (!apBufferHeader)
		{
		return OMX_ErrorBadParameter;
		}
	
	OMX_ERRORTYPE err = OMX_ErrorNone;
	if (aDirection == OMX_DirInput)
		{
		//Buffers must be a multiple of block align. Artifact of CMMFImaAdpcmToPcm16Codec::CheckPreconditions
		//EXECEPT for the last buffer when we will just throw away extra data 
		TBool lastBuffer = apBufferHeader->nFlags & OMX_BUFFERFLAG_EOS ? ETrue : EFalse;
		if( ( apBufferHeader->nFilledLen % iCore->BlockAlign() ) != 0)
			{
            if( !lastBuffer && iCore->ForgetNotAlignedInputBuffers()) //don't deal with partial input buffers (decoder) apart from the last one
                {
                return OMX_ErrorBadParameter;
                }
			}

		TInt symerr = iBuffersToEmpty.Append(apBufferHeader);
		if (symerr!=KErrNone)
			{
			err = OMX_ErrorInsufficientResources;
			}
		}
	else if (aDirection == OMX_DirOutput)
		{
		
		TInt symerr = iBuffersToFill.Append(apBufferHeader);
		if (symerr!=KErrNone)
			{
			err = OMX_ErrorInsufficientResources;
			}
		}
	else
		{
		// direction is neither input or output... bad parameter!
		err = OMX_ErrorBadParameter;
		}

	// on success, and if we're in a running state, (re)start the core to process the buffers
	if (err==OMX_ErrorNone && (iBuffersToEmpty.Count()>0 || iBuffersToFill.Count()>0) && iState==OMX_StateExecuting)
		{
		iCore->Start();
		}

	return err;
	}

OMX_BOOL COmxILAdPcmCodecProcessingFunction::BufferRemovalIndication(OMX_BUFFERHEADERTYPE* apBufferHeader, OMX_DIRTYPE aDirection)
	{
	OMX_BOOL headerreleased = OMX_FALSE;

	// select which buffer array to process based on direction
	RPointerArray<OMX_BUFFERHEADERTYPE>* bufarray = NULL;
	if (aDirection==OMX_DirInput)
		{
		bufarray = &iBuffersToEmpty;
		}
	else if (aDirection==OMX_DirOutput)
		{
		bufarray = &iBuffersToFill;
		}


	// find the buffer in the array
	TInt pos = bufarray->Find(apBufferHeader);
	if (pos!=KErrNotFound)
		{
		// if this buffer is at the head of the array, and the core is active, then the core may be using it...
		// in this case we want to reset the core (to stop it using the buffer), release the buffer, and then restart the core
		TBool bufinuse = (pos==0 && iCore->IsRunning());
		if (bufinuse)
			{
			iCore->Reset();
			}

		// remove the buffer
		bufarray->Remove(pos);
		headerreleased = OMX_TRUE;

		// if the core was reset, restart it
		if (bufinuse)
			{
			iCore->Start();
			}
		}

	return headerreleased;
	}

OMX_BUFFERHEADERTYPE* COmxILAdPcmCodecProcessingFunction::NextInputBuffer()
	{
	return (iBuffersToEmpty.Count()>0) ? iBuffersToEmpty[0] : NULL ;
	}

OMX_BUFFERHEADERTYPE* COmxILAdPcmCodecProcessingFunction::NextOutputBuffer()
	{
	return (iBuffersToFill.Count()>0) ? iBuffersToFill[0] : NULL;
	}

void COmxILAdPcmCodecProcessingFunction::InputBufferConsumed(OMX_BUFFERHEADERTYPE* aBuffer)
	{
	ASSERT(iBuffersToEmpty.Count()>0);
	ASSERT(iBuffersToEmpty[0]==aBuffer);

	iBuffersToEmpty.Remove(0);

	iCallbacks.BufferDoneNotification(aBuffer, 0, OMX_DirInput);
	}

void COmxILAdPcmCodecProcessingFunction::OutputBufferFilled(OMX_BUFFERHEADERTYPE* aBuffer, TBool aMarkWithEos)
	{
	DEBUG_PRINTF(_L8("COmxILPcmDecoderProcessingFunction::OutputBufferFilled"));

	ASSERT(iBuffersToFill.Count()>0);
	ASSERT(iBuffersToFill[0]==aBuffer);
	
	iBuffersToFill.Remove(0);

	if (aMarkWithEos)
		{
		// Propagate the EOS flag
		aBuffer->nFlags |= OMX_BUFFERFLAG_EOS;
		}

	OMX_U32 flags = aBuffer->nFlags;
	iCallbacks.BufferDoneNotification(aBuffer, 1, OMX_DirOutput);

	if (aMarkWithEos)
		{
		iCallbacks.EventNotification(
			OMX_EventBufferFlag,
			1,
			flags,
			0);
		}
	}
