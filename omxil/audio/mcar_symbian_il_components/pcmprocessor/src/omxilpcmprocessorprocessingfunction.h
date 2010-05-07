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

#ifndef OMXILPCMDECODERPROCESSINGFUNCTION_H
#define OMXILPCMDECODERPROCESSINGFUNCTION_H

#include <openmax/il/khronos/v1_x/OMX_Audio.h>
#include <openmax/il/khronos/v1_x/OMX_Types.h>
#if defined(NCP_COMMON_MM_3PA_ENABLED) || defined(SYMBIAN_MULTIMEDIA_THREEPLANEARCH)
#include <openmax/il/common/omxilprocessingfunction.h>
#else
#include "omxilprocessingfunction.h"
#endif

class COmxILPcmProcessorProcessingFunctionCore;

class MOmxILPcmProcessorProcessingFunctionCoreContext
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
		
		// callback from core on processing error
		virtual void ErrorCallback(OMX_ERRORTYPE aOmxError) = 0;
	};

class COmxILPcmProcessorProcessingFunction : public COmxILProcessingFunction, private MOmxILPcmProcessorProcessingFunctionCoreContext
	{
	private:
		class TPcmEncodingType
			{
			public:
				enum TEncoding
					{
					EPcmEncodingLinear = 0,
					EPcmEncodingMuLaw,
					EPcmEncodingALaw,
					EPcmEncodingMax
					};
				
				enum TWidthAndEndianness
					{
					EPcmWidth8bit = 0,
					EPcmWidth16bitLE,	// only for linear encodings
					EPcmWidth16bitBE,	// only for linear encodings
					EPcmWidthMax
					};
				
				enum TSignedness
					{
					EPcmSigned = 0,		// only meaningful for linear encodings
					EPcmUnsigned,		// only meaningful or linear encodings
					EPcmSignednessMax
					};
			
			public:
				inline TPcmEncodingType();
				inline TPcmEncodingType(TEncoding aEncoding, TWidthAndEndianness aWidth, TSignedness aSignedness);
				inline TInt SetFromPcmMode(OMX_AUDIO_PCMMODETYPE aPCMMode, OMX_U32 aBitsPerSample, OMX_ENDIANTYPE aEndian, OMX_NUMERICALDATATYPE aSigned);
				inline TUint32 Hash() const;
				
			public:
				// Produces a unique value for any valid PCM encoding
				// This is an arithmetic hash
				template<typename T1, typename T2, typename T3>
				inline static TUint PcmEncodingHash(T1 aEncoding, T2 aWidth, T3 aSignedness)
					{
					return
					static_cast<TUint>(aEncoding)
						+ static_cast<TUint>(TPcmEncodingType::EPcmEncodingMax)
						* ( static_cast<TUint>(aWidth)
							+ static_cast<TUint>(TPcmEncodingType::EPcmWidthMax)
							* static_cast<TUint>(aSignedness) );
					}
		
				inline static TUint PcmMaxHash();
				inline static TUint PcmPairHash(TUint aLeft, TUint aRight);
				
			public:
				TEncoding iEncoding;
				TWidthAndEndianness iWidth;
				TSignedness iSignedness;
			};

		class COmxILPcmProcessorProcessingFunctionCore : public CAsyncOneShot
			{
			public:
				// MUST be done from the same thread:
				
				static COmxILPcmProcessorProcessingFunctionCore* NewL(MOmxILPcmProcessorProcessingFunctionCoreContext& aContext);
				~COmxILPcmProcessorProcessingFunctionCore();
		
				
				TInt SetEncodings(const TPcmEncodingType& aInputEncoding, const TPcmEncodingType& aOutputEncoding);
		
				// May be called from any thread: 
				void Start();
				void Stop();
				void Reset();
				
				TBool IsRunning();
				
			private:
				COmxILPcmProcessorProcessingFunctionCore(MOmxILPcmProcessorProcessingFunctionCoreContext& aContext);
				void ConstructL();
				
				void CompleteSelf();
				
				// from CActive / CAsyncOneShot
				void DoCancel();
				void RunL();
				TInt RunError(TInt aError);
				
				// make some CActive public methods private since they're unsafe to use from different threads
				using CActive::Cancel;
				using CActive::IsActive;
				
			private:
				RMutex iLock;				// mutex to allow starting and stopping safely from differing threads
				TBool iIsRunning;			// mutex protected - MUST ACQUIRE LOCK BEFORE USE
				TBool iSignalledToRun;		// mutex protected - MUST ACQUIRE LOCK BEFORE USE
				MOmxILPcmProcessorProcessingFunctionCoreContext& iContext;
				void (*iConverter)(const TAny* aIn, TUint aInLength, TAny* aOut, TUint aOutLength, TUint& aInConsumed, TUint& aOutUsed );
			};
	
	public:
		static COmxILPcmProcessorProcessingFunction* NewL(MOmxILCallbackNotificationIf& aCallbacks);
		~COmxILPcmProcessorProcessingFunction();
		
		// from COmxILProcessingFunction
		OMX_ERRORTYPE StateTransitionIndication(COmxILFsm::TStateIndex aNewState);
		OMX_ERRORTYPE BufferFlushingIndication(TUint32 aPortIndex, OMX_DIRTYPE aDirection);
		OMX_ERRORTYPE ParamIndication(OMX_INDEXTYPE aParamIndex, const TAny* apComponentParameterStructure);
		OMX_ERRORTYPE ConfigIndication(OMX_INDEXTYPE aConfigIndex, const TAny* apComponentConfigStructure);
		OMX_ERRORTYPE BufferIndication(OMX_BUFFERHEADERTYPE* apBufferHeader, OMX_DIRTYPE aDirection);
		OMX_BOOL BufferRemovalIndication(OMX_BUFFERHEADERTYPE* apBufferHeader, OMX_DIRTYPE aDirection);		
	
	private:
		COmxILPcmProcessorProcessingFunction(MOmxILCallbackNotificationIf& aCallbacks);
		void ConstructL();

		TBool CheckAndSetConversion();
		
		// from MOmxILPcmProcessorProcessingFunctionCoreContext
		OMX_BUFFERHEADERTYPE* NextInputBuffer();
		OMX_BUFFERHEADERTYPE* NextOutputBuffer();
		void InputBufferConsumed(OMX_BUFFERHEADERTYPE* aBuffer);
		void OutputBufferFilled(OMX_BUFFERHEADERTYPE* aBuffer, TBool aMarkWithEos);
		void ErrorCallback(OMX_ERRORTYPE aOmxError);
	
		
	private:
		COmxILFsm::TStateIndex iState;
		COmxILPcmProcessorProcessingFunctionCore* iCore;
		
		OMX_AUDIO_PARAM_PCMMODETYPE iInputMode;
		OMX_AUDIO_PARAM_PCMMODETYPE iOutputMode;
		
		RPointerArray<OMX_BUFFERHEADERTYPE> iBuffersToEmpty;
		RPointerArray<OMX_BUFFERHEADERTYPE> iBuffersToFill;
	};

#include "omxilpcmprocessorprocessingfunction.inl"
#endif
