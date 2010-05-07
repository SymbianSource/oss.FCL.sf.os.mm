/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

#include "log.h"
#include "omxiladpcmcodecprocessingfunctioncore.h"

COmxILAdPcmCodecProcessingFunctionCore::COmxILAdPcmCodecProcessingFunctionCore()
	:CAsyncOneShot( EPriorityUserInput )
	{
	}

COmxILAdPcmCodecProcessingFunctionCore::~COmxILAdPcmCodecProcessingFunctionCore()
	{
	//we do not own iContext 
	Cancel();
	iLock.Close();
	
	delete [] iInternalOutputBuffer;
	}

void COmxILAdPcmCodecProcessingFunctionCore::ConstructL()
	{
	User::LeaveIfError(iLock.CreateLocal());
	}

void COmxILAdPcmCodecProcessingFunctionCore::SetObserver( MOmxILAdPcmCodecProcessingFunctionCoreContext* aContext )
	{
	iContext = aContext;
	}

void COmxILAdPcmCodecProcessingFunctionCore::Start()
	{
	DEBUG_PRINTF(_L8("COmxILAdPcmCodecProcessingFunctionCore::Start"));

	iLock.Wait();

	iIsRunning = ETrue;

	CompleteSelf();
	
	iLock.Signal();
	}

void COmxILAdPcmCodecProcessingFunctionCore::Stop()
	{
	DEBUG_PRINTF(_L8("COmxILAdPcmCodecProcessingFunctionCore::Stop"));

	iLock.Wait();
	iIsRunning = EFalse;
	iLock.Signal();
	}

void COmxILAdPcmCodecProcessingFunctionCore::Reset()
	{
	iLock.Wait();
	
	iIsRunning = EFalse;
	
	iInputBuffer = NULL;
	iOutputBuffer = NULL;
	iInternalOutputBufferHasDataToCopy = EFalse;
	iBlockAlign = KImaAdpcmBlockAlign;
	iSamplesPerBlock = KImaAdpcmSamplesPerBlock;
	
	delete [] iInternalOutputBuffer;
	iInternalOutputBuffer = NULL;
	iInternalOutputBufferSize = 0;

	iLock.Signal();
	}


TBool COmxILAdPcmCodecProcessingFunctionCore::IsRunning()
	{
	DEBUG_PRINTF(_L8("COmxILAdPcmCodecProcessingFunctionCore::IsRunning"));

	iLock.Wait();
	TBool retval = iIsRunning;
	iLock.Signal();
	
	return retval;
	}

void COmxILAdPcmCodecProcessingFunctionCore::CompleteSelf()
	{
	DEBUG_PRINTF(_L8("COmxILAdPcmCodecProcessingFunctionCore::CompleteSelf"));

	// MUST HAVE ALREADY ACQUIRED THE LOCK
	if (!iSignalledToRun)
		{
		iSignalledToRun = ETrue;
		Call();
		}
	}


void COmxILAdPcmCodecProcessingFunctionCore::DoCancel()
	{
	// do nothing
	}

void COmxILAdPcmCodecProcessingFunctionCore::RunL()
	{
	iLock.Wait();		// acquire the lock. This prevents other threads from changing run-state while we're actively processing
	iSignalledToRun = EFalse;
	
	if (iIsRunning && iStatus==KErrNone)//iStatus is always KErrNone, except for canceling
		{
		// if we don't have any input or output buffers, obtain them
		if (!iInputBuffer)
			{
			iInputBuffer = iContext->NextInputBuffer();
			}
		if (!iOutputBuffer)
			{
			iOutputBuffer = iContext->NextOutputBuffer();
			}
		
		//try to flush data with 
		TryToFlushTheResultsIfNeeded( EFalse );//it cannot be the last output buffer
		
		// if we STILL don't have an input and an output buffer, then we can't do any conversion, and don't activate ourselves
		// also if there is something to flush and there is not space in the buffers we cannot proceed
		if (!(iInputBuffer && iOutputBuffer && !iInternalOutputBufferHasDataToCopy ))
			
			{
			iIsRunning = EFalse;
			}
		else
			{
			OMX_U8* inbuf = GetInBuf();	
			
			if( inbuf )
				{
				//Note that iInternalOutputBuffer needs to be flushed to the output buffers before we call Convert, otherwise the data is lost
				Convert( inbuf, iInternalOutputBuffer, iSamplesPerBlock);
				//offsets in the input is moved in GetInBuf();
				iInternalOutputBufferOffset = 0;
				iInternalOutputBufferHasDataToCopy = ETrue;//convert always fills the whole buffer
				}
			
			// we're processing our last buffer if the input buffer is EOS flagged
			TBool lastBuffer = iInputBuffer->nFlags & OMX_BUFFERFLAG_EOS ? ETrue : EFalse;
			TBool lastOutputBuffer = EFalse;		// we may have multiple output buffers for each input. This flag will be set when we know we're done so we may propogate the EOS.

			//Start: flash input buffer if done with it
			if( 0 == iInputBuffer->nFilledLen )//we ate all src
				{
				DEBUG_PRINTF(_L8("COmxILAdPcmCodecProcessingFunctionCore::RunL finished with an Input buffer"));
				OMX_BUFFERHEADERTYPE* buf = iInputBuffer;
				
				iInputBuffer = NULL;
					
				iContext->InputBufferConsumed( buf );
				// if we're done with our EOS marked input buffer, then our output buffer is the last one
		
				lastOutputBuffer = lastBuffer;
				}
			//End: flash input buffer if done with it
			
			TryToFlushTheResultsIfNeeded( lastOutputBuffer );

			// complete ourselves - will cyummspaintontinue with the next buffer(s) if any
			CompleteSelf();
			}
		}
	iLock.Signal();		// release the lock - although we may be set to run, our runstate may be changed as long as we're not actually processing
	}


void COmxILAdPcmCodecProcessingFunctionCore::TryToFlushTheResultsIfNeeded( TBool aLastOutputBuffer )
	{
	//the code below assumes that the output buffer is '%4=0' type
	if (iInternalOutputBufferHasDataToCopy && iOutputBuffer )
		{
		const TUint internalOutputBufferSize = InternalOutputBufferSize();
		const TUint outputBufferCapacity = (iOutputBuffer->nAllocLen- (iOutputBuffer->nFilledLen + iOutputBuffer->nOffset) );
		if ( outputBufferCapacity > 0)//if there is space in the output buffer
			{
			const TInt bytesLeftInTheInternalBuffer = internalOutputBufferSize - iInternalOutputBufferOffset;
			const TInt bytesToCopy = (outputBufferCapacity > bytesLeftInTheInternalBuffer) ? bytesLeftInTheInternalBuffer : outputBufferCapacity;
			//if there is space in the output buffer for the whole reminder of the internal buffer
			Mem::Copy( iOutputBuffer->pBuffer+iOutputBuffer->nOffset+iOutputBuffer->nFilledLen//target
					, iInternalOutputBuffer + iInternalOutputBufferOffset//source
					, bytesToCopy);//length
			iOutputBuffer->nFilledLen += bytesToCopy;
			
			iInternalOutputBufferOffset += bytesToCopy;
			if (iInternalOutputBufferOffset >= internalOutputBufferSize )//reached the end of internal buffer
				{
				iInternalOutputBufferHasDataToCopy = EFalse;
				}
			}
		}
	// if the output buffer is full, release it
	if ( iOutputBuffer &&
	( aLastOutputBuffer || ( iOutputBuffer->nOffset+iOutputBuffer->nFilledLen == iOutputBuffer->nAllocLen ) ) )
		{
		DEBUG_PRINTF4(_L8("COmxILAdPcmCodecProcessingFunctionCore::RunL finished with an Output buffer: off %d, filled %d, alloc %d"), iOutputBuffer->nOffset, iOutputBuffer->nFilledLen,iOutputBuffer->nAllocLen);
		OMX_BUFFERHEADERTYPE* buf = iOutputBuffer;
		iOutputBuffer = NULL;

		iContext->OutputBufferFilled(buf, aLastOutputBuffer );
		}
	}

OMX_U8* COmxILAdPcmCodecProcessingFunctionCore::GetInBuf()
	{
	//we assume here inputBuffer is pointing to something (checked in another place)
	const TUint howMuchOneConvertEats = HowMuchOneConvertEats();
	if (ForgetNotAlignedInputBuffers())
		{//we don't deal with partial input buffers (decoder)
		if (iInputBuffer->nFilledLen < howMuchOneConvertEats)//there is less left than a full block ->use a temp buffer;
				{
				iInputBuffer->nOffset += iInputBuffer->nFilledLen;
				iInputBuffer->nFilledLen = 0;//-= iInputBuffer->nFilledLen;
				return 0;
				}
		else//there is at least one full block left
				{
				OMX_U8* const buf = iInputBuffer->pBuffer + iInputBuffer->nOffset;
				iInputBuffer->nOffset += howMuchOneConvertEats;
				iInputBuffer->nFilledLen -= howMuchOneConvertEats;
				return buf;
				}
		}
	else
		{//so we deal with partial input buffers (encoder)
		const TUint inputLeft = iInputBuffer->nFilledLen;
		if ( inputLeft > 0)//if there is space in the output buffer
			{
			const TInt spaceLeftInTempSrcBuffer = howMuchOneConvertEats - iTempSrcBufferOffset;
			const TInt bytesToCopy = (inputLeft > spaceLeftInTempSrcBuffer) ? spaceLeftInTempSrcBuffer : inputLeft;//min
			
			Mem::Copy( iTempSrcBuffer+iTempSrcBufferOffset//target
					, iInputBuffer->pBuffer+iInputBuffer->nOffset//source
					, bytesToCopy);//length
			iTempSrcBufferOffset += bytesToCopy;
			iInputBuffer->nOffset += bytesToCopy;
			iInputBuffer->nFilledLen -= bytesToCopy;
			}
		if ( iTempSrcBufferOffset == howMuchOneConvertEats )//input buffer ready?
			{
			iTempSrcBufferOffset = 0;
			return iTempSrcBuffer;
			}
		}
	return 0;
	}

TInt COmxILAdPcmCodecProcessingFunctionCore::RunError(TInt aError)
	{
	// shouldn't be possible to reach here - just return the error
	return aError;
	}
