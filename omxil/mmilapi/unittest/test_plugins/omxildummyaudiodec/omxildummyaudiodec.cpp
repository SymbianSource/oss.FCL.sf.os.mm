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


#include "omxildummyaudiodec.h"
#include "iltestuids.hrh"
#include <openmax/il/loader/omxilsymbiancomponentif.h>
#include <mmf/server/mmfdatabuffer.h>

_LIT8(KDummyDecoderILComponentName,"OMX.SYMBIAN.AUDIO.DECODER.DUMMY");
const TInt KMaxComponentNameLength = 128;

OMXIL_COMPONENT_ECOM_ENTRYPOINT(KUidOmxILDummyAudioDec);

const TUint32 KOMXALL = 0xFFFFFFFF;

const TInt KMaxComponentRoleLength = 128;

const TInt KIndexInputPort = 0;
const TInt KIndexOutputPort = 1;

// Set some dummy input and output buffer sizes
const TInt KDummyDecoderInputBufferSize 	= 0x2000;
const TInt KDummyDecoderOutputBufferSize 	= 0x4000;

//_LIT(KDummyDecoder, "DummyDecoder");
							


TInt COmxIlDummyAudioDec::CreateComponent(OMX_HANDLETYPE hComponent)
	{
	COmxIlDummyAudioDec* self = new COmxIlDummyAudioDec(hComponent);
	if (self==NULL)
		{
		return KErrNoMemory;
		
		}
	else
		{
		return KErrNone;
		}
	}
	
OMX_ERRORTYPE COmxIlDummyAudioDec::GetComponentVersion(
       OMX_STRING pComponentName,
       OMX_VERSIONTYPE* pComponentVersion,
       OMX_VERSIONTYPE* pSpecVersion,
       OMX_UUIDTYPE* pComponentUUID)
	{
	TPtr8 name(reinterpret_cast<TUint8*>(pComponentName),
			   KMaxComponentNameLength);
	name.Copy(KDummyDecoderILComponentName);
	name.PtrZ();

	OMX_VERSIONTYPE compVer = { 1,0,0,0 };
	OMX_VERSIONTYPE specVer = { 1,1,1,0 };

	(*pComponentVersion) 	= compVer;
	(*pSpecVersion)	  		= specVer;

	TPtr8 thisUid(reinterpret_cast<TUint8*>(*pComponentUUID), 128); //Maxsize fixed by Khronos;
	thisUid = TPtr8(reinterpret_cast<TUint8*>(this), sizeof(TUint32));

	return OMX_ErrorNone;
	}


COmxIlDummyAudioDec::COmxIlDummyAudioDec(OMX_HANDLETYPE hComponent)
	:COmxComponentImpl(hComponent)
	{
	iState = OMX_StateLoaded;
	}
	
COmxIlDummyAudioDec::~COmxIlDummyAudioDec()
	{
	if (iState == OMX_StateExecuting)
		{
		iState = OMX_StateIdle;
		}
	}

OMX_ERRORTYPE COmxIlDummyAudioDec::SendCommand(
       OMX_COMMANDTYPE Cmd,
       TUint32 nParam1,
       TAny* /*pCmdData*/)
	{
	OMX_ERRORTYPE error = OMX_ErrorNone;
	switch (Cmd)
		{
		case OMX_CommandStateSet:
			{
			OMX_STATETYPE state = (OMX_STATETYPE)nParam1;
			if (state == iState)
				{
				error = OMX_ErrorSameState;
				}
			else
				{
				// notify client of the state change
				switch (state)
					{
				case OMX_StateIdle:
				case OMX_StatePause:
				case OMX_StateExecuting:
					break;
					};
				iState = state;
				
				EventHandlerCallback(
					OMX_EventCmdComplete,
					OMX_CommandStateSet,
					iState,
					NULL);	
				}			
			break;
			}

		case OMX_CommandFlush:
			{
			if(nParam1 == KOMXALL)
				{
				EventHandlerCallback(
						OMX_EventCmdComplete,
						OMX_CommandFlush,
						0,
						NULL);	

				EventHandlerCallback(
						OMX_EventCmdComplete,
						OMX_CommandFlush,
						1,
						NULL);	
				}
			else
				{
				EventHandlerCallback(
						OMX_EventCmdComplete,
						OMX_CommandFlush,
						nParam1,
						NULL);	
				}
				
			break;
			}
		case OMX_CommandPortDisable:
			{
			if(nParam1 == KOMXALL)
				{
				EventHandlerCallback(
						OMX_EventCmdComplete,
						OMX_CommandPortDisable,
						0,
						NULL);	

				EventHandlerCallback(
						OMX_EventCmdComplete,
						OMX_CommandPortDisable,
						1,
						NULL);	
				}
			else
				{
				EventHandlerCallback(
						OMX_EventCmdComplete,
						OMX_CommandPortDisable,
						nParam1,
						NULL);	
				}
			break;
			}
		}	
	return error;
	}
	
OMX_ERRORTYPE COmxIlDummyAudioDec::GetParameter(
       OMX_INDEXTYPE nParamIndex,  
       TAny* ComponentParameterStructure)
	{
	switch (nParamIndex)
		{
	case OMX_IndexParamAudioInit :
		{
		OMX_PORT_PARAM_TYPE* param = static_cast<OMX_PORT_PARAM_TYPE*>(ComponentParameterStructure);
		param->nPorts = 2;
		}
		break;
	case OMX_IndexParamPortDefinition:
		{
		OMX_PARAM_PORTDEFINITIONTYPE* portDef = static_cast<OMX_PARAM_PORTDEFINITIONTYPE*>(ComponentParameterStructure);
		if (portDef->nPortIndex==0)	
			{
			portDef->eDir = OMX_DirInput;	
			portDef->nBufferSize = KDummyDecoderInputBufferSize;
			portDef->nBufferCountActual = iBufferCountActual0;	
			}
		else
			{
			portDef->eDir = OMX_DirOutput;
			portDef->nBufferSize = KDummyDecoderOutputBufferSize;
			portDef->nBufferCountActual = iBufferCountActual1;	
			}
		}
		break;
	default:
		return OMX_ErrorUnsupportedIndex;
		}
	return OMX_ErrorNone;
	}
	
OMX_ERRORTYPE COmxIlDummyAudioDec::SetParameter(
       OMX_INDEXTYPE nIndex,
       TAny* ComponentParameterStructure)
	{
	ASSERT(iState == OMX_StateLoaded);
	switch (nIndex)
		{
		case OMX_IndexParamAudioVorbis:
			{
			// Fake this is Audio Vorbis decoder for the purpose of testing
			OMX_AUDIO_PARAM_VORBISTYPE* param = static_cast<OMX_AUDIO_PARAM_VORBISTYPE*>(ComponentParameterStructure);
			switch(param->nPortIndex)
				{
				case 0: // Input port
					{
					// nothing to set
					return OMX_ErrorNone;
					}			
				case 1: // Output port
				default:
					{
					return OMX_ErrorUnsupportedIndex;	
					}
				};
			}
		case OMX_IndexParamAudioPcm:
			{
			OMX_AUDIO_PARAM_PCMMODETYPE* param = static_cast<OMX_AUDIO_PARAM_PCMMODETYPE*>(ComponentParameterStructure);
			switch(param->nPortIndex)
				{
				case 1: // Output port = PCM
					{
					return OMX_ErrorNone;
					}
				case 0: // Input port
				default:
					{
					return OMX_ErrorUnsupportedIndex;	
					}
				};
			}
		case OMX_IndexParamPortDefinition:
			{
			OMX_PARAM_PORTDEFINITIONTYPE* portDef = static_cast<OMX_PARAM_PORTDEFINITIONTYPE*>(ComponentParameterStructure);
			if (portDef->nPortIndex==0)	
				{
				iBufferCountActual0 = portDef->nBufferCountActual;	
				}
			else
				{
				iBufferCountActual1 = portDef->nBufferCountActual;	
				}
			return OMX_ErrorNone;
			}
		default:
			{
			return OMX_ErrorUnsupportedIndex;
			}
		};		
	}
	
OMX_ERRORTYPE COmxIlDummyAudioDec::GetConfig(
       OMX_INDEXTYPE /*nIndex*/, 
       TAny* /*value*/)
	{
	return OMX_ErrorUnsupportedIndex;
	}
	
OMX_ERRORTYPE COmxIlDummyAudioDec::SetConfig(
       OMX_INDEXTYPE /*nIndex*/, 
       TAny* /*value*/)
	{
	return OMX_ErrorUnsupportedIndex;
	}
	
OMX_ERRORTYPE COmxIlDummyAudioDec::GetExtensionIndex(
       OMX_STRING /*parameterName*/,
       OMX_INDEXTYPE* pIndexType)
	{
	// This test component returns always 	OMX_IndexParamPortDefinition
	*pIndexType = OMX_IndexParamPortDefinition;

	return OMX_ErrorNone;
	}
	
OMX_ERRORTYPE COmxIlDummyAudioDec::GetState(
       OMX_STATETYPE* pState)
	{
	*pState = iState;
	return OMX_ErrorNone;
	}
	
// To be implemented for DM4
OMX_ERRORTYPE COmxIlDummyAudioDec::ComponentTunnelRequest(
		OMX_HANDLETYPE /*hInput*/,
		TUint32 /*nInputPort*/,
		OMX_HANDLETYPE /*hOutput*/,
		TUint32 /*nOutputPort*/,
		OMX_TUNNELSETUPTYPE* /*pTunnelSetup*/)
	{

	// Fake everything as if it went fine 
	return OMX_ErrorNone;
	}
	
OMX_ERRORTYPE COmxIlDummyAudioDec::UseBuffer(
       OMX_BUFFERHEADERTYPE** ppBufferHeader,
       TUint32 nPortIndex,
       TAny* pAppPrivate,
       TUint32 nSizeBytes,
       TUint8* pBuffer)
	{
	ASSERT(iState == OMX_StateLoaded);
	*ppBufferHeader = new OMX_BUFFERHEADERTYPE;
	if (*ppBufferHeader != NULL)
		{
		(*ppBufferHeader)->pBuffer = pBuffer;
		(*ppBufferHeader)->pAppPrivate = pAppPrivate;
		(*ppBufferHeader)->nAllocLen = nSizeBytes;
		(*ppBufferHeader)->nFilledLen = 0;
		(*ppBufferHeader)->nFlags = 0;
		(*ppBufferHeader)->pInputPortPrivate = NULL;
		(*ppBufferHeader)->pOutputPortPrivate = NULL;
		}


		
	if (*ppBufferHeader)
		{
		TPtr8 ptr(pBuffer,nSizeBytes,nSizeBytes);
		CMMFBuffer* buffer = NULL;
		TRAPD(err, buffer = CMMFPtrBuffer::NewL(ptr));
		if (err != KErrNone)
			{
			return OMX_ErrorInsufficientResources;
			}
		switch (nPortIndex)
			{
		case KIndexInputPort:
				{
				(*ppBufferHeader)->pInputPortPrivate = buffer;
				(*ppBufferHeader)->nInputPortIndex = nPortIndex;
				}
				break;
			case KIndexOutputPort:
				{
				(*ppBufferHeader)->pOutputPortPrivate = buffer;	
				(*ppBufferHeader)->nOutputPortIndex = nPortIndex;
				}
				break;
			
			}
		return OMX_ErrorNone;
		}
	else
		{
		return OMX_ErrorInsufficientResources;
		}
	}
	
OMX_ERRORTYPE COmxIlDummyAudioDec::AllocateBuffer(
		OMX_BUFFERHEADERTYPE** pBuffer,
		TUint32 nPortIndex,
		TAny* pAppData,
		TUint32 nSizeBytes)
	{
	ASSERT(iState == OMX_StateLoaded);
	
	*pBuffer = new OMX_BUFFERHEADERTYPE;
	if (*pBuffer != NULL)
		{
		CMMFDescriptorBuffer* buffer = NULL;
		TRAPD(err, buffer = CMMFDescriptorBuffer::NewL(nSizeBytes));
		if (err != KErrNone)
			{
			return OMX_ErrorInsufficientResources;
			}
		(*pBuffer)->pBuffer = const_cast<TUint8*>(buffer->Data().Ptr());
		// store our allocated memory in component's private store
		switch (nPortIndex)
			{
		case KIndexInputPort:
			(*pBuffer)->pInputPortPrivate = buffer;
			(*pBuffer)->pOutputPortPrivate = NULL;
			break;
		case KIndexOutputPort:
			(*pBuffer)->pOutputPortPrivate = buffer;
			(*pBuffer)->pInputPortPrivate = NULL;
			break;
			};
		
		
		(*pBuffer)->nAllocLen = nSizeBytes;
		(*pBuffer)->nFilledLen = 0;
		(*pBuffer)->pAppPrivate = pAppData;
		}
		
	if (*pBuffer && (*pBuffer)->pBuffer)
		{
		return OMX_ErrorNone;
		}
	else
		{
		return OMX_ErrorInsufficientResources;
		}
	}

OMX_ERRORTYPE COmxIlDummyAudioDec::FreeBuffer(
		TUint32 nPortIndex,
       OMX_BUFFERHEADERTYPE* pBuffer)
	{
	switch (nPortIndex) 
		{		
		case KIndexInputPort:
			{
			delete (static_cast<CMMFBuffer*>(pBuffer->pInputPortPrivate));
			pBuffer->pInputPortPrivate = NULL;
			break;
			}
		case KIndexOutputPort:
			delete (static_cast<CMMFBuffer*>(pBuffer->pOutputPortPrivate));
			pBuffer->pOutputPortPrivate = NULL;
			break;	
			
		}
	delete pBuffer;
	return OMX_ErrorNone;
	}
OMX_ERRORTYPE COmxIlDummyAudioDec::EmptyThisBuffer(
       OMX_BUFFERHEADERTYPE* pBuffer)
	{
	ASSERT(iState == OMX_StateExecuting ||
			iState == OMX_StateIdle ||
			iState == OMX_StatePause);
	EmptyBufferDoneCallback(pBuffer);
	return OMX_ErrorNone;
	}
OMX_ERRORTYPE COmxIlDummyAudioDec::FillThisBuffer(
           OMX_BUFFERHEADERTYPE* pBuffer)
	{
	ASSERT(iState == OMX_StateExecuting ||
			iState == OMX_StateIdle ||
			iState == OMX_StatePause);

	FillBufferDoneCallback(pBuffer);
	return OMX_ErrorNone;
	}
	
OMX_ERRORTYPE COmxIlDummyAudioDec::SetCallbacks(
           OMX_CALLBACKTYPE* pCallbacks, 
           TAny* pAppData)
	{
	iCallback = pCallbacks;
	iAppData = pAppData;
	return OMX_ErrorNone;
	}

OMX_ERRORTYPE COmxIlDummyAudioDec::ComponentRoleEnum(
			OMX_OUT OMX_U8 *aRole,
			OMX_IN OMX_U32 aIndex)
	{
	_LIT8(KFakeRole,"audio_decoder.vrb");

	if (aIndex == 0)
		{
		TPtr8 ptr((TUint8*)aRole, 0, KMaxComponentRoleLength);
		ptr.Copy(KFakeRole);
		ptr.PtrZ();
		
		return OMX_ErrorNone;
		}
	else
		{
		return OMX_ErrorBadParameter;
		}
	}
	
// Callbacks for the Dummy Audio Decoder
void COmxIlDummyAudioDec::EventHandlerCallback( 
        			OMX_OUT OMX_EVENTTYPE eEvent, 
        			OMX_OUT TUint32 nData1,
        			OMX_OUT TUint32 nData2,
        			OMX_OUT OMX_STRING cExtraInfo)
	{
	iCallback->EventHandler(
			this,
			iAppData,
			eEvent,
			nData1,
			nData2,
			cExtraInfo);	
	}
	
void COmxIlDummyAudioDec::FillBufferDoneCallback(OMX_BUFFERHEADERTYPE* aBuffer)
	{
	iCallback->FillBufferDone(
		*this,
		iAppData,
		aBuffer);
	}
	
void COmxIlDummyAudioDec::EmptyBufferDoneCallback(OMX_BUFFERHEADERTYPE* aBuffer)
	{
	iCallback->EmptyBufferDone(
		*this,
		iAppData,
		aBuffer);		
	}
	
// Component Entry Point
OMX_ERRORTYPE OMX_ComponentInit(OMX_HANDLETYPE hComponent)
	{
	TInt err = COmxIlDummyAudioDec::CreateComponent(hComponent);
	if (err == KErrNone)
		return OMX_ErrorNone;
	else 
		{
		// return some problem
		return OMX_ErrorInsufficientResources;
		
		}
	}

