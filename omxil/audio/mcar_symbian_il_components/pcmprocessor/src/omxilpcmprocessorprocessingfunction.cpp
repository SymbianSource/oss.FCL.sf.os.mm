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
#if defined(NCP_COMMON_MM_3PA_ENABLED) || defined(SYMBIAN_MULTIMEDIA_THREEPLANEARCH)
#include <openmax/il/common/log.h>
#include <openmax/il/common/omxilcallbacknotificationif.h>
#else
#include "log.h"
#include "omxilcallbacknotificationif.h"
#endif

#include "omxilpcmprocessorprocessingfunction.h"
#include "omxilpcmprocessorconverters.h"


#include "omxilpcmprocessor.h"
#include <openmax/il/common/omxilspecversion.h>

// Will install itself in the constructing threads ActiveScheduler.
COmxILPcmProcessorProcessingFunction::COmxILPcmProcessorProcessingFunctionCore* COmxILPcmProcessorProcessingFunction::COmxILPcmProcessorProcessingFunctionCore::NewL(MOmxILPcmProcessorProcessingFunctionCoreContext& aContext)
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunctionCore::NewL"));

	
	COmxILPcmProcessorProcessingFunctionCore* self = new(ELeave)COmxILPcmProcessorProcessingFunctionCore(aContext);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

COmxILPcmProcessorProcessingFunction::COmxILPcmProcessorProcessingFunctionCore::COmxILPcmProcessorProcessingFunctionCore(MOmxILPcmProcessorProcessingFunctionCoreContext& aContext)
	: CAsyncOneShot(EPriorityUserInput), iContext(aContext)
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunctionCore::COmxILPcmProcessorProcessingFunctionCore"));
	}
	
void COmxILPcmProcessorProcessingFunction::COmxILPcmProcessorProcessingFunctionCore::ConstructL()
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunctionCore::ConstructL"));
	User::LeaveIfError(iLock.CreateLocal());
	}
	

COmxILPcmProcessorProcessingFunction::COmxILPcmProcessorProcessingFunctionCore::~COmxILPcmProcessorProcessingFunctionCore()
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunctionCore::~COmxILPcmProcessorProcessingFunctionCore"));
	Cancel();
	iLock.Close();
	}

// Set the input and output encodings. Returns KErrNone on success, or KErrNotSupported if the conversion isn't supported
// May be done by any thread but only while the core is inactive
TInt COmxILPcmProcessorProcessingFunction::COmxILPcmProcessorProcessingFunctionCore::SetEncodings(const TPcmEncodingType& aInputEncoding, const TPcmEncodingType& aOutputEncoding)
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunctionCore::SetEncodings"));

	// select the conversion function based on the input and output encodings
	// this uses the unique hash values for the encodings
	
	TUint32 inputhash = aInputEncoding.Hash();
	TUint32 outputhash = aOutputEncoding.Hash();
	
	// if our input and output are the same, then no conversion is necessary
	if (inputhash==outputhash)
		{
		iConverter = &NOmxILPcmProcessorConverters::PCMConverterNoConversion;
		}
	else
		{
		TUint32 combinedhash = TPcmEncodingType::PcmPairHash( inputhash, outputhash );
		
		if (combinedhash == TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth8bit, TPcmEncodingType::EPcmUnsigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmSigned)) 
			|| combinedhash == TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth8bit, TPcmEncodingType::EPcmUnsigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitBE, TPcmEncodingType::EPcmSigned)))
			{
			iConverter = &NOmxILPcmProcessorConverters::PCMConverterU8toS16;
			}
		
		else if (combinedhash == TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmUnsigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmSigned))) 
			{
			iConverter = &NOmxILPcmProcessorConverters::PCMConverterU16LEtoS16LE;
			}
			
		else if (combinedhash == TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmSigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth8bit, TPcmEncodingType::EPcmUnsigned))) 
			{
			iConverter = &NOmxILPcmProcessorConverters::PCMConverterS16LEtoU8;
			}
			
		else if (combinedhash == TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitBE, TPcmEncodingType::EPcmUnsigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmSigned)))
			{
			iConverter = &NOmxILPcmProcessorConverters::PCMConverterU16BEtoS16LE;
			}
			
		else if (combinedhash == TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmSigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth8bit, TPcmEncodingType::EPcmSigned)))
			{
			iConverter = &NOmxILPcmProcessorConverters::PCMConverterS16LEtoS8;
			}
			
		else if (combinedhash == TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth8bit, TPcmEncodingType::EPcmSigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmSigned)))
			{
			iConverter = &NOmxILPcmProcessorConverters::PCMConverterS8toS16LE;
			}
			
		else if (combinedhash == TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmSigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmUnsigned)))
			{
			iConverter = &NOmxILPcmProcessorConverters::PCMConverterS16LEtoU16LE;
			}
			
		else if (combinedhash == TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmSigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitBE, TPcmEncodingType::EPcmUnsigned)))
			{
			iConverter = &NOmxILPcmProcessorConverters::PCMConverterS16LEtoU16BE;
			}
			
		else if (combinedhash == TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmSigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitBE, TPcmEncodingType::EPcmSigned))
			|| combinedhash == TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitBE, TPcmEncodingType::EPcmSigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmSigned))
			|| combinedhash == TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmUnsigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitBE, TPcmEncodingType::EPcmUnsigned))
			|| combinedhash ==  TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitBE, TPcmEncodingType::EPcmUnsigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmUnsigned)))
			{
			iConverter = &NOmxILPcmProcessorConverters::PCMConverterS16SwapEndianness;
			}
			
		else if (combinedhash == TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingALaw, TPcmEncodingType::EPcmWidth8bit, TPcmEncodingType::EPcmUnsigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmSigned))
			|| combinedhash ==  TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingALaw, TPcmEncodingType::EPcmWidth8bit, TPcmEncodingType::EPcmSigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmSigned)))
			{
			iConverter = &NOmxILPcmProcessorConverters::PCMConverterALawtoS16LE;
			}
			
		else if (combinedhash == TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmSigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingALaw, TPcmEncodingType::EPcmWidth8bit, TPcmEncodingType::EPcmUnsigned))
			|| combinedhash ==  TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmSigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingALaw, TPcmEncodingType::EPcmWidth8bit, TPcmEncodingType::EPcmSigned)))
			{
			iConverter = &NOmxILPcmProcessorConverters::PCMConverterS16LEtoALaw;
			}
			
		else if (combinedhash == TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingMuLaw, TPcmEncodingType::EPcmWidth8bit, TPcmEncodingType::EPcmUnsigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmSigned))
			|| combinedhash ==  TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingMuLaw, TPcmEncodingType::EPcmWidth8bit, TPcmEncodingType::EPcmSigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmSigned)))
			{
			iConverter = &NOmxILPcmProcessorConverters::PCMConverterMuLawtoS16LE;
			}
			
		else if (combinedhash == TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmSigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingMuLaw, TPcmEncodingType::EPcmWidth8bit, TPcmEncodingType::EPcmUnsigned))
			|| combinedhash ==  TPcmEncodingType::PcmPairHash(TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingLinear, TPcmEncodingType::EPcmWidth16bitLE, TPcmEncodingType::EPcmSigned), TPcmEncodingType::PcmEncodingHash(TPcmEncodingType::EPcmEncodingMuLaw, TPcmEncodingType::EPcmWidth8bit, TPcmEncodingType::EPcmSigned)))
			{
			iConverter = &NOmxILPcmProcessorConverters::PCMConverterS16LEtoMuLaw;
			}
		else
			{
			return KErrNotSupported;
			}
		}
		
	return KErrNone;
	}

void COmxILPcmProcessorProcessingFunction::COmxILPcmProcessorProcessingFunctionCore::Start()
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunctionCore::Start"));

	iLock.Wait();

	iIsRunning = ETrue;
	CompleteSelf();
	
	iLock.Signal();
	}

void COmxILPcmProcessorProcessingFunction::COmxILPcmProcessorProcessingFunctionCore::Stop()
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunctionCore::Stop"));

	iLock.Wait();
	iIsRunning = EFalse;
	iLock.Signal();
	}

void COmxILPcmProcessorProcessingFunction::COmxILPcmProcessorProcessingFunctionCore::Reset()
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunctionCore::Reset"));

	Stop();
	}

TBool COmxILPcmProcessorProcessingFunction::COmxILPcmProcessorProcessingFunctionCore::IsRunning()
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunctionCore::IsRunning"));

	iLock.Wait();
	TBool retval = iIsRunning;
	iLock.Signal();
	
	return retval;
	}

void COmxILPcmProcessorProcessingFunction::COmxILPcmProcessorProcessingFunctionCore::CompleteSelf()
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunctionCore::CompleteSelf"));

	// MUST HAVE ALREADY ACQUIRED THE LOCK
	if (!iSignalledToRun)
		{
		iSignalledToRun = ETrue;
		Call();
		}
	}

void COmxILPcmProcessorProcessingFunction::COmxILPcmProcessorProcessingFunctionCore::DoCancel()
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunctionCore::DoCancel"));
	// do nothing
	}

void COmxILPcmProcessorProcessingFunction::COmxILPcmProcessorProcessingFunctionCore::RunL()
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunctionCore::RunL"));

	iLock.Wait();		// acquire the lock. This prevents other threads from changing run-state while we're actively processing
	iSignalledToRun = EFalse;
	
	if (iIsRunning && iStatus==KErrNone)
		{
		
		// obtain the input or output buffers
		OMX_BUFFERHEADERTYPE* inputbuffer = iContext.NextInputBuffer();
		OMX_BUFFERHEADERTYPE* outputbuffer = iContext.NextOutputBuffer();

		// if we STILL don't have an input and an output buffer, then stop
		// otherwise, process the buffers
		if (!inputbuffer || !outputbuffer)
			{
			iIsRunning = EFalse;
			}
		else
			{
			// convert
			OMX_U8* inbuf = inputbuffer->pBuffer + inputbuffer->nOffset;
			TUint inlen = inputbuffer->nFilledLen;
			
			OMX_U8* outbuf = outputbuffer->pBuffer + outputbuffer->nFilledLen;
			TUint outlen = outputbuffer->nAllocLen - outputbuffer->nFilledLen;
			
			// copy the timestamp from the input to the output buffer header
			outputbuffer->nTimeStamp = inputbuffer->nTimeStamp;
			
            // we're processing our last buffer if the input buffer is EOS flagged
            TBool lastBuffer = inputbuffer->nFlags & OMX_BUFFERFLAG_EOS ? ETrue : EFalse;			
            if ((inlen == 0 && !lastBuffer) || outlen == 0)
                {
                iContext.ErrorCallback(OMX_ErrorUnderflow);
				iLock.Signal();	
				return;
                }			
			
			TUint inconsumed = 0;
			TUint outused = 0;
			iConverter(inbuf, inlen, outbuf, outlen, inconsumed, outused);
			ASSERT((inconsumed && outused) || lastBuffer);
			
			// adjust input consumed offset and remaining length
			inputbuffer->nOffset += inconsumed;
			inputbuffer->nFilledLen -= inconsumed;
			
			// adjust output used filled length
			outputbuffer->nFilledLen += outused;
		
			TBool lastOutputBuffer = EFalse;		// we may have multiple output buffers for each input. This flag will be set when we know we're done so we may propogate the EOS.
			
			// if the input buffer is completely consumed, release it
			if (0 == inputbuffer->nFilledLen)
				{
				iContext.InputBufferConsumed(inputbuffer);
				
				// if we're done with our EOS marked input buffer, then our output buffer is the last one
				lastOutputBuffer = lastBuffer;
				}
		
			// if the output buffer is full, or it is the last one, release it
			if ( lastOutputBuffer ||
					(outputbuffer->nFilledLen == outputbuffer->nAllocLen)
				)
				{
				iContext.OutputBufferFilled(outputbuffer, lastOutputBuffer);
				}

			// complete ourselves to continue with the next buffer(s) if any
			CompleteSelf();
			}
		}

	iLock.Signal();		// release the lock - although we may be set to run, our runstate may be changed as long as we're not actually processing
	}

TInt COmxILPcmProcessorProcessingFunction::COmxILPcmProcessorProcessingFunctionCore::RunError(TInt aError)
	{
	// shouldn't be possible to reach here - just return the error
	return aError;
	}

COmxILPcmProcessorProcessingFunction* COmxILPcmProcessorProcessingFunction::NewL(MOmxILCallbackNotificationIf& aCallbacks)
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunction::NewL"));

	COmxILPcmProcessorProcessingFunction* self = new(ELeave)COmxILPcmProcessorProcessingFunction(aCallbacks);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	
	return self;
	}

COmxILPcmProcessorProcessingFunction::COmxILPcmProcessorProcessingFunction(MOmxILCallbackNotificationIf& aCallbacks)
	: COmxILProcessingFunction(aCallbacks)
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunction::COmxILPcmProcessorProcessingFunction"));
	}

void COmxILPcmProcessorProcessingFunction::ConstructL()
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunction::ConstructL"));
	
	// Default settings
	OMX_VERSIONTYPE ver = TOmxILSpecVersion(); // CONSTANT
	iInputMode.nSize = sizeof(OMX_AUDIO_PARAM_PCMMODETYPE);
	iInputMode.nVersion = ver;
	iInputMode.nPortIndex = 0;
	iInputMode.nChannels = 1;
	iInputMode.eNumData = OMX_NumericalDataSigned;
	iInputMode.eEndian = OMX_EndianLittle;
	iInputMode.bInterleaved = OMX_TRUE;
	iInputMode.nBitPerSample = 16;
	iInputMode.nSamplingRate = 48000;
	iInputMode.ePCMMode = OMX_AUDIO_PCMModeLinear;
	iInputMode.eChannelMapping[0] = OMX_AUDIO_ChannelLF;
	iInputMode.eChannelMapping[1] = OMX_AUDIO_ChannelRF;
	
	// Same default settings for output mode, except port
	iOutputMode = iInputMode;
	iOutputMode.nPortIndex = 1;
	
	iCore = COmxILPcmProcessorProcessingFunctionCore::NewL(*this);
	}

COmxILPcmProcessorProcessingFunction::~COmxILPcmProcessorProcessingFunction()
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunction::~COmxILPcmProcessorProcessingFunction"));

	delete iCore;
	
	iBuffersToEmpty.Reset();
	iBuffersToFill.Reset();
	}


OMX_ERRORTYPE COmxILPcmProcessorProcessingFunction::StateTransitionIndication(COmxILFsm::TStateIndex aNewState)
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunction::StateTransitionIndication"));

	OMX_ERRORTYPE err = OMX_ErrorNone;
	switch(aNewState)
		{

		case COmxILFsm::EStateIdle:
			{
			iCore->Reset();
			
			// Make sure settings have been applied
			// This should never fail since the settings are defaults or
			// have already been checked.
			TInt symerr = CheckAndSetConversion();
			ASSERT(symerr ==KErrNone);
			
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
		case COmxILFsm::ESubStateIdleToLoaded:
		case COmxILFsm::ESubStateExecutingToIdle:
		case COmxILFsm::ESubStatePauseToIdle:
			// do nothing
			break;
		}
	
	iState = aNewState;
	return err;
	}

OMX_ERRORTYPE COmxILPcmProcessorProcessingFunction::BufferFlushingIndication(TUint32 aPortIndex, OMX_DIRTYPE aDirection)
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunction::BufferFlushingIndication"));

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

OMX_ERRORTYPE COmxILPcmProcessorProcessingFunction::ParamIndication(OMX_INDEXTYPE aParamIndex, const TAny* apComponentParameterStructure)
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunction::ParamIndication"));

	OMX_ERRORTYPE err = OMX_ErrorNone;
	
    switch(aParamIndex)
    	{
    	case OMX_IndexParamAudioPcm:
    		{
    		const OMX_AUDIO_PARAM_PCMMODETYPE* pPcmProfile = static_cast<const OMX_AUDIO_PARAM_PCMMODETYPE*>(apComponentParameterStructure);
    		
    		// check the static settings
    		// settings which must match between input and output (i.e. sample rate) are checked by CheckAndSetConversion()
    		ASSERT ((pPcmProfile->nPortIndex == 0 || pPcmProfile->nPortIndex == 1)
    				&& (pPcmProfile->nChannels == 1 || pPcmProfile->nChannels == 2)
    				&& (pPcmProfile->nBitPerSample == 8 || pPcmProfile->nBitPerSample == 16)
    				&& (pPcmProfile->eChannelMapping[0] == OMX_AUDIO_ChannelLF)
    				&& (pPcmProfile->eChannelMapping[1] == OMX_AUDIO_ChannelRF)
    				);
    		
    		// Rememeber the original settings in case we need to revert
    		OMX_AUDIO_PARAM_PCMMODETYPE origPortIn = iInputMode;
    		OMX_AUDIO_PARAM_PCMMODETYPE origPortOut = iOutputMode;
    		
			// Preserve the settings
			if (pPcmProfile->nPortIndex == 0)
				{
				iInputMode = *pPcmProfile;
				DEBUG_PRINTF(_L8("INFO: Input port changed."));
				}
			else
				{
				iOutputMode = *pPcmProfile;
				DEBUG_PRINTF(_L8("INFO: Output port changed."));
				}
			
			// Apply the settings
			TInt validSettings = CheckAndSetConversion();
			    		    		
			// If the settings do not match modify the other port (the one that
			// has not just been set) to match the current port, since the
			// conversion between like and like is always valid.
			if (validSettings != KErrNone)
				{
				if (pPcmProfile->nPortIndex == 0)
					{
					iOutputMode = *pPcmProfile;
					DEBUG_PRINTF2(_L8("WARNING: Port mismatch (code %d), had to reconfigure output port."), validSettings);
					}
				else
					{
					iInputMode = *pPcmProfile;
					DEBUG_PRINTF2(_L8("WARNING: Port mismatch (code %d), had to reconfigure input port."), validSettings);
					}
				
				// Apply the settings again
				validSettings = CheckAndSetConversion();
				
				// If the settings still aren't valid restore the original
				// settings and return a failiure.
				if (validSettings != KErrNone)
					{
					DEBUG_PRINTF2(_L8("WARNING: Port changes invalid (code %d), reverting."), validSettings);
					iInputMode = origPortIn;
					iOutputMode = origPortOut;
					return OMX_ErrorBadParameter;
					}
				
				// Generate an event informing the user the settings have been
				// changed on both ports.
				iCallbacks.EventNotification(OMX_EventPortSettingsChanged, OMX_IndexParamPortDefinition, 0, NULL);
				iCallbacks.EventNotification(OMX_EventPortSettingsChanged, OMX_IndexParamPortDefinition, 1, NULL);
				}
			else
				{
				DEBUG_PRINTF(_L8("INFO: Port changed successfully."));
				// Generate an event informing the user the settings have been
				// changed on the requested port.
				iCallbacks.EventNotification(OMX_EventPortSettingsChanged, OMX_IndexParamPortDefinition, pPcmProfile->nPortIndex, NULL);
				}
    		}
    		break;
    	
    	default:
    		{
    		// Ignore
    		}
    		break;
    	}
	
    return err;
	}

OMX_ERRORTYPE COmxILPcmProcessorProcessingFunction::ConfigIndication(OMX_INDEXTYPE /*aConfigIndex*/, const TAny* /*apComponentConfigStructure*/)
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunction::ConfigIndication"));

	// no configs are processable for this codec... just return success
	return OMX_ErrorNone;
	}

OMX_ERRORTYPE COmxILPcmProcessorProcessingFunction::BufferIndication(OMX_BUFFERHEADERTYPE* apBufferHeader, OMX_DIRTYPE aDirection)
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunction::BufferIndication"));

	// Assert valid parameters
	ASSERT(apBufferHeader);
	ASSERT(apBufferHeader->nOffset == 0);
	ASSERT(aDirection == OMX_DirInput || aDirection == OMX_DirOutput);
	
	OMX_ERRORTYPE err = OMX_ErrorNone;
	if (aDirection == OMX_DirInput)
		{
		TInt symerr = iBuffersToEmpty.Append(apBufferHeader);
		if (symerr!=KErrNone)
			{
			err = OMX_ErrorInsufficientResources;
			}
		}
	else if (aDirection == OMX_DirOutput)
		{
		// Clear the flags for output buffers. We'll set them as necessary before sending them along
		apBufferHeader->nFlags=0;
		TInt symerr = iBuffersToFill.Append(apBufferHeader);
		if (symerr!=KErrNone)
			{
			err = OMX_ErrorInsufficientResources;
			}
		}
	
	// on success, and if we're in a running state, (re)start the core to process the buffers
	if (err==OMX_ErrorNone && iBuffersToEmpty.Count()>0 && iBuffersToFill.Count()>0 && iState==OMX_StateExecuting)
		{
		iCore->Start();
		}
	
	return err;
	}

OMX_BOOL COmxILPcmProcessorProcessingFunction::BufferRemovalIndication(OMX_BUFFERHEADERTYPE* apBufferHeader, OMX_DIRTYPE aDirection)
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunction::BufferRemovalIndication"));
	// TODO: Check whether headerreleased is required
	//OMX_BOOL headerreleased = OMX_FALSE;

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
		//headerreleased = OMX_TRUE;
		
		// if the core was reset, restart it
		if (bufinuse)
			{
			iCore->Start();
			}
		}
	
	//DEBUG_PRINTF2(_L8("COmxILPcmProcessorProcessingFunction::BufferRemovalIndication buffer removed [%d]"), (TInt)headerreleased);
	//------HACK-------
	//return headerreleased;
	return OMX_TRUE;
	//-----------------
	}

TInt COmxILPcmProcessorProcessingFunction::CheckAndSetConversion()
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunction::CheckAndSetConversion"));
	
	// Assert ports are incompatible
	if ((iInputMode.nSamplingRate != iOutputMode.nSamplingRate)
				|| (iInputMode.nChannels != iOutputMode.nChannels)
				|| (iInputMode.bInterleaved != iOutputMode.bInterleaved))
		{
		return KErrNotSupported;
		}
	
	// create TPcmEncodingType objects for input and output and set the conversion to use on the core
	TInt err = KErrNone;
	TPcmEncodingType in;
	TPcmEncodingType out;
	
	if (!err)
		{
		err = in.SetFromPcmMode(iInputMode.ePCMMode, iInputMode.nBitPerSample, iInputMode.eEndian, iInputMode.eNumData);
		}
	if (!err)
		{
		err = out.SetFromPcmMode(iOutputMode.ePCMMode, iOutputMode.nBitPerSample, iOutputMode.eEndian, iOutputMode.eNumData);
		}
	
	if (!err)
		{
		err = iCore->SetEncodings(in, out);
		}
	
	return err;
	}

OMX_BUFFERHEADERTYPE* COmxILPcmProcessorProcessingFunction::NextInputBuffer()
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunction::NextInputBuffer"));

	if (iBuffersToEmpty.Count()>0) return iBuffersToEmpty[0];
	else return NULL;
	}

OMX_BUFFERHEADERTYPE* COmxILPcmProcessorProcessingFunction::NextOutputBuffer()
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunction::NextOutputBuffer"));

	if (iBuffersToFill.Count()>0) return iBuffersToFill[0];
	else return NULL;
	}

void COmxILPcmProcessorProcessingFunction::InputBufferConsumed(OMX_BUFFERHEADERTYPE* aBuffer)
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunction::InputBufferConsumed"));

	ASSERT(iBuffersToEmpty.Count()>0);
	ASSERT(iBuffersToEmpty[0]==aBuffer);
	
	iBuffersToEmpty.Remove(0);
	// Reset the offset back to 0, it was used for marking the read position in the processing loop
	aBuffer->nOffset = 0;
	iCallbacks.BufferDoneNotification(aBuffer, 0, OMX_DirInput);
	}

void COmxILPcmProcessorProcessingFunction::OutputBufferFilled(OMX_BUFFERHEADERTYPE* aBuffer, TBool aMarkWithEos)
	{
	DEBUG_PRINTF(_L8("COmxILPcmProcessorProcessingFunction::OutputBufferFilled"));

	ASSERT(iBuffersToFill.Count()>0);
	ASSERT(iBuffersToFill[0]==aBuffer);
	
	iBuffersToFill.Remove(0);

	if (aMarkWithEos)
		{
		// Propagate the EOS flag
		aBuffer->nFlags |= OMX_BUFFERFLAG_EOS;
		}
	
	iCallbacks.BufferDoneNotification(aBuffer, 1, OMX_DirOutput);

	if (aMarkWithEos)
		{
		OMX_U32 flags = aBuffer->nFlags;		
		iCallbacks.EventNotification(
			OMX_EventBufferFlag,
			1,
			flags,
			0);
		}
	}

void COmxILPcmProcessorProcessingFunction::ErrorCallback(OMX_ERRORTYPE aOmxError)
	{
	iCallbacks.ErrorEventNotification(aOmxError);
	}
