// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef OMXILDUMMYAUDIODEC_H
#define OMXILDUMMYAUDIODEC_H

#include "omximpl.h"

// Test component. Dummy omx il decoder used by the MILIF test suite.
class COmxIlDummyAudioDec : public COmxComponentImpl
	{
public:
	friend class CCodecDummyProcessor;
	friend class CCodecCallbacks;

	static TInt CreateComponent(OMX_HANDLETYPE hComponent);
	~COmxIlDummyAudioDec();
	
	OMX_ERRORTYPE GetComponentVersion(
			OMX_STRING pComponentName,
			OMX_VERSIONTYPE* pComponentVersion,
			OMX_VERSIONTYPE* pSpecVersion,
			OMX_UUIDTYPE* pComponentUUID);
    OMX_ERRORTYPE SendCommand(
			OMX_COMMANDTYPE Cmd,
			TUint32 nParam1,
			TAny* pCmdData);
    OMX_ERRORTYPE GetParameter(
			OMX_INDEXTYPE nParamIndex,  
			TAny* ComponentParameterStructure);
    OMX_ERRORTYPE SetParameter(
			OMX_INDEXTYPE nIndex,
			TAny* ComponentParameterStructure);
    OMX_ERRORTYPE GetConfig(
			OMX_INDEXTYPE nIndex, 
			TAny* value);
    OMX_ERRORTYPE SetConfig(
			OMX_INDEXTYPE nIndex, 
			TAny* value);
    OMX_ERRORTYPE GetExtensionIndex(
			OMX_STRING ParameterName,
			OMX_INDEXTYPE* pIndexType);
    OMX_ERRORTYPE GetState(
			OMX_STATETYPE* pState);
    OMX_ERRORTYPE ComponentTunnelRequest(
			OMX_HANDLETYPE hInput,
			TUint32 nInputPort,
			OMX_HANDLETYPE hOutput,
			TUint32 nOutputPort,
			OMX_TUNNELSETUPTYPE* pTunnelSetup);
    virtual OMX_ERRORTYPE UseBuffer(
			OMX_BUFFERHEADERTYPE** pBufferHdr,
			TUint32 nPortIndex,
			TAny* pAppPrivate,
			TUint32 nSizeBytes,
			TUint8* pBuffer);
    virtual OMX_ERRORTYPE AllocateBuffer(
           OMX_BUFFERHEADERTYPE** pBuffer,
           TUint32 nPortIndex,
           TAny* pAppData,
           TUint32 nSizeBytes);
    OMX_ERRORTYPE FreeBuffer(
    		  TUint32 nPortIndex,	
              OMX_BUFFERHEADERTYPE* pBuffer);
    OMX_ERRORTYPE EmptyThisBuffer(
              OMX_BUFFERHEADERTYPE* pBuffer);
    OMX_ERRORTYPE FillThisBuffer(
              OMX_BUFFERHEADERTYPE* pBuffer);
    OMX_ERRORTYPE SetCallbacks(
              OMX_CALLBACKTYPE* pCallbacks, 
              TAny* pAppData);
	OMX_ERRORTYPE ComponentRoleEnum(
			OMX_OUT OMX_U8 *cRole,
			OMX_IN OMX_U32 nIndex);
              
            
    void EventHandlerCallback(
        			 OMX_EVENTTYPE eEvent, 
        			 TUint32 nData1,
        			 TUint32 nData2,
        			 OMX_STRING cExtraInfo);
    void FillBufferDoneCallback(OMX_BUFFERHEADERTYPE* pBuffer);
    void EmptyBufferDoneCallback(OMX_BUFFERHEADERTYPE* pBuffer);
            
private:
	COmxIlDummyAudioDec(OMX_HANDLETYPE hComponent);

private:
	OMX_CALLBACKTYPE* iCallback;
	TAny* iAppData;
	OMX_STATETYPE iState;
	
	TInt iBufferCountActual0; 
	TInt iBufferCountActual1;
	};

#endif // OMXILDUMMYAUDIODEC_H
