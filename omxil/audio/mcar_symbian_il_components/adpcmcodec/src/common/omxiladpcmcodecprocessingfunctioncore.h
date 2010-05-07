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

#ifndef OMXILADPCMDECODERPROCESSINGFUNCTIONCORE_H
#define OMXILADPCMDECODERPROCESSINGFUNCTIONCORE_H

#include <e32base.h>
#include "OmxILAdPcmCodecConsts.h"

#include <openmax/il/khronos/v1_x/OMX_Audio.h>
#include <openmax/il/khronos/v1_x/OMX_Types.h>

class MOmxILAdPcmCodecProcessingFunctionCoreContext
	{
public:
	// provide the core with the next input buffer to process, or NULL if there are no more
	virtual OMX_BUFFERHEADERTYPE* NextInputBuffer() = 0;

	// provide the core with the next output buffer to fill, or NULL if there are no more
	virtual OMX_BUFFERHEADERTYPE* NextOutputBuffer() = 0;

	// callback from core when an input buffer has been consumed
	virtual void InputBufferConsumed(OMX_BUFFERHEADERTYPE* aBuffer) = 0;

	// callback from core when an output buffer has been filled
	virtual void OutputBufferFilled(OMX_BUFFERHEADERTYPE* aBuffer, TBool aMarkWithEos) = 0;
	};

class COmxILAdPcmCodecProcessingFunctionCore : public CAsyncOneShot
	{
public:
	~COmxILAdPcmCodecProcessingFunctionCore();

	void SetObserver(MOmxILAdPcmCodecProcessingFunctionCoreContext* aContext); 
	// May be called from any thread. 
	void Start();
	void Stop();
	virtual void Reset();
	virtual TInt Configure() = 0;
	
	TBool IsRunning();
	void SetSamplesPerBlock(TInt aSamples ) { iSamplesPerBlock = aSamples; }
	
	virtual TInt CheckConversion() = 0;
	virtual TUint BitsPerSample() const = 0;
	virtual TUint SampleRate() const = 0;
	virtual TUint Channels() const = 0;
	virtual TUint InternalOutputBufferSize() const = 0;
	virtual TUint HowMuchOneConvertEats() const = 0;
	virtual TBool ForgetNotAlignedInputBuffers() const = 0;
    virtual OMX_ERRORTYPE SetBlockAlign(TInt aBlockAlign) = 0;
    TInt BlockAlign() const { return iBlockAlign; } 
    
protected:
	virtual void Convert( const TAny* aSrc, TAny* aDst, TInt aSamples ) = 0;	
	
protected:
	COmxILAdPcmCodecProcessingFunctionCore();
	void ConstructL();

	void CompleteSelf();

	// from CActive / CAsyncOneShot
	void DoCancel();
	void RunL();
	TInt RunError(TInt aError);
	
	// make some CActive public methods private since they're unsafe to use from different threads
	using CActive::Cancel;
	using CActive::IsActive;
	
	TUint iBlockAlign;
	TUint iSamplesPerBlock;

   OMX_U8* iInternalOutputBuffer;
   TInt iInternalOutputBufferSize;
   
private:
	OMX_U8* GetInBuf();
	void TryToFlushTheResultsIfNeeded( TBool aLastBuffer );

private:
	MOmxILAdPcmCodecProcessingFunctionCoreContext* iContext;//not owned

	OMX_BUFFERHEADERTYPE* iInputBuffer;
	OMX_BUFFERHEADERTYPE* iOutputBuffer;
	
	OMX_U8 iTempSrcBuffer[KImaAdpcmTempBufferSize]; 
	TUint iTempSrcBufferOffset;
	TUint iInternalOutputBufferOffset;
	TBool iInternalOutputBufferHasDataToCopy;
	
	RMutex iLock;				// mutex to allow starting and stopping safely from differing threads

	TBool iIsRunning;			// mutex protected - MUST ACQUIRE LOCK BEFORE USE
	TBool iSignalledToRun;		// mutex protected - MUST ACQUIRE LOCK BEFORE USE

	};

#endif// OMXILADPCMDECODERPROCESSINGFUNCTIONCORE_H_
