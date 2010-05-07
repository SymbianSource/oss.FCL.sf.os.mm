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

/**
 @file
 @internalComponent
*/

#include "log.h"
#include "omxilcomponent.h"
#include "omxilfsm.h"
#include "omxilcallbackmanager.h"
#include "omxilconfigmanager.h"


EXPORT_C
COmxILComponent::COmxILComponent()
	{
    DEBUG_PRINTF(_L8("COmxILComponent::COmxILComponent"));

	}


EXPORT_C
COmxILComponent::~COmxILComponent()
	{
    DEBUG_PRINTF2(_L8("COmxILComponent::~COmxILComponent : Handle[%08X]"), ipHandle);
	iComponentName.Close();
	}


EXPORT_C void
COmxILComponent::InitComponentL()
	{
    DEBUG_PRINTF2(_L8("COmxILComponent::InitComponent : Handle[%08X]"), ipHandle);

	__ASSERT_ALWAYS(ipHandle && ipFsm && ipConfigManager, User::Invariant());

	char componentNamebuffer[OMX_MAX_STRINGNAME_SIZE];
	OMX_VERSIONTYPE componentVersion;
	OMX_VERSIONTYPE specVersion;
	OMX_UUIDTYPE componentUid;

	ipConfigManager->GetComponentVersion(componentNamebuffer,
										 &componentVersion,
										 &specVersion,
										 &componentUid);
	TBuf8<128> componentNameBuf8;
	componentNameBuf8 = const_cast<const TUint8*>(reinterpret_cast<TUint8*>(componentNamebuffer));
	iComponentName.CreateL(componentNameBuf8, componentNameBuf8.Length() + 1);
	iComponentName.PtrZ();
    DEBUG_PRINTF2(_L8("COmxILComponent::InitComponent : [%S]"), &iComponentName);

	// Fill in the component handle
	ipHandle->nVersion				 = componentVersion;
	// The FSM will take care of all the API calls
	ipHandle->pComponentPrivate		 = ipFsm;
	ipHandle->GetComponentVersion	 = COmxILComponent::GetComponentVersion;
	ipHandle->SendCommand			 = COmxILComponent::SendCommand;
	ipHandle->GetParameter			 = COmxILComponent::GetParameter;
	ipHandle->SetParameter			 = COmxILComponent::SetParameter;
	ipHandle->GetConfig				 = COmxILComponent::GetConfig;
	ipHandle->SetConfig				 = COmxILComponent::SetConfig;
	ipHandle->GetExtensionIndex		 = COmxILComponent::GetExtensionIndex;
	ipHandle->GetState				 = COmxILComponent::GetState;
	ipHandle->ComponentTunnelRequest = COmxILComponent::ComponentTunnelRequest;
	ipHandle->UseBuffer				 = COmxILComponent::UseBuffer;
	ipHandle->AllocateBuffer		 = COmxILComponent::AllocateBuffer;
	ipHandle->FreeBuffer			 = COmxILComponent::FreeBuffer;
	ipHandle->EmptyThisBuffer		 = COmxILComponent::EmptyThisBuffer;
	ipHandle->FillThisBuffer		 = COmxILComponent::FillThisBuffer;
	ipHandle->SetCallbacks			 = COmxILComponent::SetCallbacks;
	ipHandle->ComponentDeInit		 = COmxILComponent::ComponentDeInit;
	ipHandle->UseEGLImage			 = COmxILComponent::UseEGLImage;
	ipHandle->ComponentRoleEnum		 = COmxILComponent::ComponentRoleEnum;

	OMX_ERRORTYPE omxRetValue =
		ipCallbackManager->RegisterComponentHandle(ipHandle);
	if (OMX_ErrorNone != omxRetValue)
		{
		if (OMX_ErrorInsufficientResources == omxRetValue)
			{
			User::Leave(KErrNoMemory);
			}
		User::Leave(KErrGeneral);
		}

	// Let's init the FSM...
	ipFsm->InitFsm();

	}

OMX_ERRORTYPE
COmxILComponent::GetComponentVersion(
	OMX_HANDLETYPE aComponent,
	OMX_STRING aComponentName,
	OMX_VERSIONTYPE* apComponentVersion,
	OMX_VERSIONTYPE* apSpecVersion,
	OMX_UUIDTYPE* apComponentUUID)
	{
	OMX_TRACE_GETCOMPONENTVERSION_IN(aComponent, aComponentName, aComponentVersion, aSpecVersion, aComponentUUID);
    DEBUG_PRINTF2(_L8("COmxILComponent::GetComponentVersion : Handle[%08X]"), aComponent);
    OMX_ERRORTYPE omxError;
    omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		GetComponentVersion(aComponentName, apComponentVersion,
							apSpecVersion, apComponentUUID);
    OMX_TRACE_GETCOMPONENTVERSION_OUT(aComponent, aComponentName, aComponentVersion, aSpecVersion, aComponentUUID, omxError);
    return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::SendCommand(
	OMX_HANDLETYPE aComponent,
	OMX_COMMANDTYPE aCmd,
	OMX_U32 aParam1,
	OMX_PTR aCmdData)
	{
	OMX_TRACE_SENDCOMMAND_IN(aComponent, aCmd, aParam1, aCmdData);
    DEBUG_PRINTF2(_L8("COmxILComponent::SendCommand : Handle[%08X]"), aComponent);
    OMX_ERRORTYPE omxError;
	omxError =  (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		SendCommand(aCmd ,aParam1, aCmdData);
	
	OMX_TRACE_SENDCOMMAND_OUT(aComponent, aCmd, aParam1, aCmdData, omxError);
	return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::GetParameter(
	OMX_HANDLETYPE aComponent,
	OMX_INDEXTYPE aParamIndex,
	OMX_PTR aComponentParameterStructure)
	{
	OMX_TRACE_GETPARAMETER_IN(aComponent, aParamIndex, aComponentParameterStructure);
    DEBUG_PRINTF3(_L8("COmxILComponent::GetParameter : Handle[%08X]; ParamIndex[0x%08X]"), aComponent, aParamIndex);
    
    OMX_ERRORTYPE omxError;
	omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		GetParameter(aParamIndex, aComponentParameterStructure);
	
	OMX_TRACE_GETPARAMETER_OUT(aComponent, aParamIndex, aComponentParameterStructure, omxError);
	return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::SetParameter(
	OMX_HANDLETYPE aComponent,
	OMX_INDEXTYPE aIndex,
	OMX_PTR aComponentParameterStructure)
	{
	OMX_TRACE_SETPARAMETER_IN(aComponent, aIndex, aComponentParameterStructure);
    DEBUG_PRINTF3(_L8("COmxILComponent::SetParameter : Handle[%08X]; ParamIndex[0x%08X]"), aComponent, aIndex);
    
    OMX_ERRORTYPE omxError;
	omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		SetParameter(aIndex,
					 const_cast<const TAny*>(aComponentParameterStructure));

	OMX_TRACE_SETPARAMETER_OUT(aComponent, aIndex, aComponentParameterStructure, omxError);
	return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::GetConfig(
	OMX_HANDLETYPE aComponent,
	OMX_INDEXTYPE aIndex,
	OMX_PTR aComponentParameterStructure)
	{
	OMX_TRACE_GETCONFIG_IN(aComponent, aIndex, aComponentParameterStructure);
    DEBUG_PRINTF3(_L8("COmxILComponent::GetConfig : Handle[%08X]; ConfigIndex[0x%08X]"), aComponent, aIndex);
    OMX_ERRORTYPE omxError;
	omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		GetConfig(aIndex, aComponentParameterStructure);
	OMX_TRACE_GETCONFIG_OUT(aComponent, aIndex, aComponentParameterStructure, omxError);
	return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::SetConfig(
	OMX_HANDLETYPE aComponent,
	OMX_INDEXTYPE aIndex,
	OMX_PTR aComponentConfigStructure)
	{
	OMX_TRACE_SETCONFIG_IN(aComponent, aIndex, aComponentConfigStructure);
    DEBUG_PRINTF3(_L8("COmxILComponent::SetConfig : Handle[%08X]; ConfigIndex[0x%08X]"), aComponent, aIndex);
    OMX_ERRORTYPE omxError;
	omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		SetConfig(aIndex, const_cast<const TAny*>(aComponentConfigStructure));
	OMX_TRACE_SETCONFIG_OUT(aComponent, aIndex, aComponentConfigStructure, omxError);
	return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::GetExtensionIndex(
	OMX_HANDLETYPE aComponent,
	OMX_STRING aParameterName,
	OMX_INDEXTYPE* aIndexType)
	{
	OMX_TRACE_GETEXTENSIONINDEX_IN(aComponent, aParameterName, aIndexType);	
    DEBUG_PRINTF2(_L8("COmxILComponent::GetExtensionIndex : Handle[%08X]"), aComponent);
    OMX_ERRORTYPE omxError;
	omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		GetExtensionIndex(aParameterName, aIndexType);
	OMX_TRACE_GETEXTENSIONINDEX_OUT(aComponent, aParameterName, aIndexType, omxError);
	return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::GetState(
	OMX_HANDLETYPE aComponent,
	OMX_STATETYPE* aState)
	{
	OMX_TRACE_GETSTATE_IN(aComponent, aState);
    DEBUG_PRINTF2(_L8("COmxILComponent::GetState : Handle[%08X]"), aComponent);
    OMX_ERRORTYPE omxError;
    omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		GetState(aState);
    OMX_TRACE_GETSTATE_OUT(aComponent, aState, omxError);
    return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::ComponentTunnelRequest(
	OMX_HANDLETYPE aComponent,
	OMX_U32 aPort,
	OMX_HANDLETYPE aTunneledComp,
	OMX_U32 aTunneledPort,
	OMX_TUNNELSETUPTYPE* aTunnelSetup)
	{
	OMX_TRACE_COMPONENTTUNNELREQUEST_IN(aComponent, aPort, aTunneledComp, aTunneledPort, aTunnelSetup);
    DEBUG_PRINTF2(_L8("COmxILComponent::ComponentTunnelRequest : Handle[%08X]"), aComponent);
    OMX_ERRORTYPE omxError;
	omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		ComponentTunnelRequest(aPort, aTunneledComp,
							   aTunneledPort, aTunnelSetup);
	OMX_TRACE_COMPONENTTUNNELREQUEST_OUT(aComponent, aPort, aTunneledComp, aTunneledPort, aTunnelSetup, omxError);
    return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::UseBuffer(
	OMX_HANDLETYPE aComponent,
	OMX_BUFFERHEADERTYPE** appBufferHdr,
	OMX_U32 aPortIndex,
	OMX_PTR apAppPrivate,
	OMX_U32 aSizeBytes,
	OMX_U8* aBuffer)
	{
	OMX_TRACE_USEBUFFER_IN(aComponent, appBufferHdr, aPortIndex, apAppPrivate, aSizeBytes, aBuffer);
    DEBUG_PRINTF2(_L8("COmxILComponent::UseBuffer : Handle[%08X]"), aComponent);
    OMX_ERRORTYPE omxError;
    omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		UseBuffer(appBufferHdr, aPortIndex, apAppPrivate, aSizeBytes, aBuffer);
    OMX_TRACE_USEBUFFER_OUT(aComponent, appBufferHdr, aPortIndex, apAppPrivate, aSizeBytes, aBuffer, omxError);
    return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::AllocateBuffer(
	OMX_HANDLETYPE aComponent,
	OMX_BUFFERHEADERTYPE** apBuffer,
	OMX_U32 aPortIndex,
	OMX_PTR aAppData,
	OMX_U32 aSizeBytes)
	{
	OMX_TRACE_ALLOCATEBUFFER_IN(aComponent, apBuffer, aPortIndex, aAppData, aSizeBytes);
    DEBUG_PRINTF2(_L8("COmxILComponent::AllocateBuffer : Handle[%08X]"), aComponent);
    OMX_ERRORTYPE omxError;
    omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		AllocateBuffer(apBuffer, aPortIndex, aAppData, aSizeBytes);
    OMX_TRACE_ALLOCATEBUFFER_OUT(aComponent, apBuffer, aPortIndex, aAppData, aSizeBytes, omxError);
    return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::FreeBuffer(
	OMX_HANDLETYPE aComponent,
	OMX_U32 aPortIndex,
	OMX_BUFFERHEADERTYPE* aBuffer)
	{
	OMX_TRACE_FREEBUFFER_IN(aComponent, aPortIndex, aBuffer);
    DEBUG_PRINTF2(_L8("COmxILComponent::FreeBuffer : Handle[%08X]"), aComponent);
    OMX_ERRORTYPE omxError;
    omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		FreeBuffer(aPortIndex, aBuffer);
    OMX_TRACE_FREEBUFFER_OUT(aComponent, aPortIndex, aBuffer, omxError);
    return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::EmptyThisBuffer(
	OMX_HANDLETYPE aComponent,
	OMX_BUFFERHEADERTYPE* aBuffer)
	{
	OMX_TRACE_EMPTYTHISBUFFER_IN(aComponent, aBuffer);
    DEBUG_PRINTF2(_L8("COmxILComponent::EmptyThisBuffer : Handle[%08X]"), aComponent);
#ifdef _OMXIL_COMMON_BUFFER_TRACING_ON
	RDebug::Print(_L("COmxILComponent::EmptyThisBuffer component=0x%08X header=0x%08X port=%d flags=0x%X filledLen=%d timeStamp=%Ld"),
		aComponent, aBuffer, aBuffer->nInputPortIndex, aBuffer->nFlags, aBuffer->nFilledLen, aBuffer->nTimeStamp);
#endif
	OMX_ERRORTYPE omxError;
	omxError =  (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		EmptyThisBuffer(aBuffer);
	
	OMX_TRACE_EMPTYTHISBUFFER_OUT(aComponent, aBuffer, omxError);
	return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::FillThisBuffer(
	OMX_HANDLETYPE aComponent,
	OMX_BUFFERHEADERTYPE* aBuffer)
	{
	OMX_TRACE_FILLTHISBUFFER_IN(aComponent, aBuffer);
    DEBUG_PRINTF2(_L8("COmxILComponent::FillThisBuffer : Handle[%08X]"), aComponent);
#ifdef _OMXIL_COMMON_BUFFER_TRACING_ON
	RDebug::Print(_L("COmxILComponent::FillThisBuffer component=0x%08X header=0x%08X port=%d"), aComponent, aBuffer, aBuffer->nOutputPortIndex);
#endif
	
	OMX_ERRORTYPE omxError;
	omxError =  (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		FillThisBuffer(aBuffer);
	
	OMX_TRACE_FILLTHISBUFFER_OUT(aComponent, aBuffer, omxError);
	return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::SetCallbacks(
	OMX_HANDLETYPE aComponent,
	OMX_CALLBACKTYPE* aCallbacks,
	OMX_PTR aAppData)
	{
	OMX_TRACE_SETCALLBACKS_IN(aComponent, aCallbacks, aAppData);	
    DEBUG_PRINTF2(_L8("COmxILComponent::SetCallbacks : Handle[%08X]"), aComponent);
    OMX_ERRORTYPE omxError;
	omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		SetCallbacks(const_cast<const OMX_CALLBACKTYPE*>(aCallbacks), aAppData);
	OMX_TRACE_SETCALLBACKS_OUT(aComponent, aCallbacks, aAppData, omxError);
	return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::ComponentDeInit(
	OMX_HANDLETYPE aComponent)
	{
	OMX_TRACE_COMPONENTDEINIT_IN(aComponent);
    DEBUG_PRINTF2(_L8("COmxILComponent::ComponentDeInit : Handle[%08X]"), aComponent);
	delete (static_cast<COmxILComponent*>(
				(static_cast<COmxILFsm*>(
					(static_cast<OMX_COMPONENTTYPE*>(aComponent))->
					pComponentPrivate))->GetComponent()));
	OMX_TRACE_COMPONENTDEINIT_OUT(aComponent, OMX_ErrorNone);
	return OMX_ErrorNone;
	}

OMX_ERRORTYPE
COmxILComponent::UseEGLImage(
		OMX_HANDLETYPE aComponent,
		OMX_BUFFERHEADERTYPE** appBufferHdr,
		OMX_U32 aPortIndex,
		OMX_PTR aAppPrivate,
		void* eglImage)
	{
	OMX_TRACE_USEEGLIMAGE_IN(aComponent, appBufferHdr, aPortIndex, aAppPrivate, eglImage);
    DEBUG_PRINTF2(_L8("COmxILComponent::UseEGLImage : Handle[%08X]"), aComponent);
    OMX_ERRORTYPE omxError;
    omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		UseEGLImage(appBufferHdr, aPortIndex, aAppPrivate, eglImage);
    OMX_TRACE_USEEGLIMAGE_OUT(aComponent, appBufferHdr, aPortIndex, aAppPrivate, eglImage, omxError);
    return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::ComponentRoleEnum(
	OMX_HANDLETYPE aComponent,
	OMX_U8* aRole,
	OMX_U32 aIndex)
	{
	OMX_TRACE_COMPONENTROLEENUM_IN(aComponent, aRole, aIndex);
    DEBUG_PRINTF2(_L8("COmxILComponent::ComponentRoleEnum : Handle[%08X]"), aComponent);
    OMX_ERRORTYPE omxError;
    omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		ComponentRoleEnum(aRole, aIndex);
    OMX_TRACE_COMPONENTROLEENUM_OUT(aComponent, aRole, aIndex, omxError);
    return omxError;
	}

/*static*/ EXPORT_C OMX_ERRORTYPE COmxILComponent::SymbianErrorToGetHandleError(TInt aSymbianError)
	{
	switch(aSymbianError)
		{
	case KErrNone:
		return OMX_ErrorNone;
	case KErrNoMemory:
		return OMX_ErrorInsufficientResources;
	case KErrArgument:
		return OMX_ErrorBadParameter;
		};

	return OMX_ErrorUndefined;	
	}
