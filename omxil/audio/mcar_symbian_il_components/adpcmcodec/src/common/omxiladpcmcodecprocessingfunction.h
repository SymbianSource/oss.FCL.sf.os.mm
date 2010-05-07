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

#ifndef OMXILADPCMCODECPOCESSINGFUNCTION_H
#define OMXILADPCMCODECPOCESSINGFUNCTION_H

#include <openmax/il/khronos/v1_x/OMX_Audio.h>
#include <openmax/il/khronos/v1_x/OMX_Types.h>
#include "omxilprocessingfunction.h"
#include "omxiladpcmcodecprocessingfunctioncore.h"

class COmxILAdPcmCodecProcessingFunction : public COmxILProcessingFunction, private MOmxILAdPcmCodecProcessingFunctionCoreContext
	{
public:
	static COmxILAdPcmCodecProcessingFunction* NewL(COmxILAdPcmCodecProcessingFunctionCore* aCore, MOmxILCallbackNotificationIf& aCallbacks);
	~COmxILAdPcmCodecProcessingFunction();

	// from COmxILProcessingFunction
	OMX_ERRORTYPE StateTransitionIndication(COmxILFsm::TStateIndex aNewState);
	OMX_ERRORTYPE BufferFlushingIndication(TUint32 aPortIndex, OMX_DIRTYPE aDirection);
	OMX_ERRORTYPE ParamIndication(OMX_INDEXTYPE aParamIndex, const TAny* apComponentParameterStructure);
	OMX_ERRORTYPE ConfigIndication(OMX_INDEXTYPE aConfigIndex, const TAny* apComponentConfigStructure);
	OMX_ERRORTYPE BufferIndication(OMX_BUFFERHEADERTYPE* apBufferHeader, OMX_DIRTYPE aDirection);
	OMX_BOOL BufferRemovalIndication(OMX_BUFFERHEADERTYPE* apBufferHeader, OMX_DIRTYPE aDirection);
	
private:
	COmxILAdPcmCodecProcessingFunction(COmxILAdPcmCodecProcessingFunctionCore* aCore, MOmxILCallbackNotificationIf& aCallbacks);
	void ConstructL();

	// from MOmxILAdPcmCodecProcessingFunctionCoreContext
	OMX_BUFFERHEADERTYPE* NextInputBuffer();
	OMX_BUFFERHEADERTYPE* NextOutputBuffer();
	void InputBufferConsumed(OMX_BUFFERHEADERTYPE* aBuffer);
	void OutputBufferFilled(OMX_BUFFERHEADERTYPE* aBuffer, TBool aMarkWithEos);


private:
	COmxILFsm::TStateIndex iState;
	COmxILAdPcmCodecProcessingFunctionCore* iCore;

	RPointerArray<OMX_BUFFERHEADERTYPE> iBuffersToEmpty;
	RPointerArray<OMX_BUFFERHEADERTYPE> iBuffersToFill;
	};


#endif /*OMXILADPCMCODECPOCESSINGFUNCTION_H_*/
