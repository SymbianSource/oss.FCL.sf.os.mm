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
    DEBUG_PRINTF2(_L8("COmxILComponent::~COmxILComponent : Handle[%X]"), ipHandle);
	iComponentName.Close();
	}


EXPORT_C void
COmxILComponent::InitComponentL()
	{
    DEBUG_PRINTF2(_L8("COmxILComponent::InitComponent : Handle[%X]"), ipHandle);

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
    DEBUG_PRINTF2(_L8("COmxILComponent::GetComponentVersion : Handle[%X]"), aComponent);
    OMX_ERRORTYPE omxError;
    omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		GetComponentVersion(aComponentName, apComponentVersion,
							apSpecVersion, apComponentUUID);
    return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::SendCommand(
	OMX_HANDLETYPE aComponent,
	OMX_COMMANDTYPE aCmd,
	OMX_U32 aParam1,
	OMX_PTR aCmdData)
	{
    DEBUG_PRINTF2(_L8("COmxILComponent::SendCommand : Handle[%X]"), aComponent);
    OMX_ERRORTYPE omxError;
	omxError =  (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		SendCommand(aCmd ,aParam1, aCmdData);
	
	return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::GetParameter(
	OMX_HANDLETYPE aComponent,
	OMX_INDEXTYPE aParamIndex,
	OMX_PTR aComponentParameterStructure)
	{
    DEBUG_PRINTF3(_L8("COmxILComponent::GetParameter : Handle[%X]; ParamIndex[0x%X]"), aComponent, aParamIndex);
    
    OMX_ERRORTYPE omxError;
	omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		GetParameter(aParamIndex, aComponentParameterStructure);
	
	return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::SetParameter(
	OMX_HANDLETYPE aComponent,
	OMX_INDEXTYPE aIndex,
	OMX_PTR aComponentParameterStructure)
	{
    DEBUG_PRINTF3(_L8("COmxILComponent::SetParameter : Handle[%X]; ParamIndex[0x%X]"), aComponent, aIndex);
    
    OMX_ERRORTYPE omxError;
	omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		SetParameter(aIndex,
					 const_cast<const TAny*>(aComponentParameterStructure));

	return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::GetConfig(
	OMX_HANDLETYPE aComponent,
	OMX_INDEXTYPE aIndex,
	OMX_PTR aComponentParameterStructure)
	{
    DEBUG_PRINTF3(_L8("COmxILComponent::GetConfig : Handle[%X]; ConfigIndex[0x%X]"), aComponent, aIndex);
    OMX_ERRORTYPE omxError;
	omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		GetConfig(aIndex, aComponentParameterStructure);
	return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::SetConfig(
	OMX_HANDLETYPE aComponent,
	OMX_INDEXTYPE aIndex,
	OMX_PTR aComponentConfigStructure)
	{
    DEBUG_PRINTF3(_L8("COmxILComponent::SetConfig : Handle[%X]; ConfigIndex[0x%X]"), aComponent, aIndex);
    OMX_ERRORTYPE omxError;
	omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		SetConfig(aIndex, const_cast<const TAny*>(aComponentConfigStructure));
	return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::GetExtensionIndex(
	OMX_HANDLETYPE aComponent,
	OMX_STRING aParameterName,
	OMX_INDEXTYPE* aIndexType)
	{
    DEBUG_PRINTF2(_L8("COmxILComponent::GetExtensionIndex : Handle[%X]"), aComponent);
    OMX_ERRORTYPE omxError;
	omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		GetExtensionIndex(aParameterName, aIndexType);
	return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::GetState(
	OMX_HANDLETYPE aComponent,
	OMX_STATETYPE* aState)
	{
    DEBUG_PRINTF2(_L8("COmxILComponent::GetState : Handle[%X]"), aComponent);
    OMX_ERRORTYPE omxError;
    omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		GetState(aState);
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
    DEBUG_PRINTF2(_L8("COmxILComponent::ComponentTunnelRequest : Handle[%X]"), aComponent);
    OMX_ERRORTYPE omxError;
	omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		ComponentTunnelRequest(aPort, aTunneledComp,
							   aTunneledPort, aTunnelSetup);
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
    DEBUG_PRINTF2(_L8("COmxILComponent::UseBuffer : Handle[%X]"), aComponent);
    OMX_ERRORTYPE omxError;
    omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		UseBuffer(appBufferHdr, aPortIndex, apAppPrivate, aSizeBytes, aBuffer);
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
    DEBUG_PRINTF2(_L8("COmxILComponent::AllocateBuffer : Handle[%X]"), aComponent);
    OMX_ERRORTYPE omxError;
    omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		AllocateBuffer(apBuffer, aPortIndex, aAppData, aSizeBytes);
    return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::FreeBuffer(
	OMX_HANDLETYPE aComponent,
	OMX_U32 aPortIndex,
	OMX_BUFFERHEADERTYPE* aBuffer)
	{
    DEBUG_PRINTF2(_L8("COmxILComponent::FreeBuffer : Handle[%X]"), aComponent);
    OMX_ERRORTYPE omxError;
    omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		FreeBuffer(aPortIndex, aBuffer);
    return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::EmptyThisBuffer(
	OMX_HANDLETYPE aComponent,
	OMX_BUFFERHEADERTYPE* aBuffer)
	{
    DEBUG_PRINTF2(_L8("COmxILComponent::EmptyThisBuffer : Handle[%X]"), aComponent);
#ifdef _OMXIL_COMMON_BUFFER_TRACING_ON
	RDebug::Print(_L("COmxILComponent::EmptyThisBuffer component=0x%08X header=0x%08X port=%d flags=0x%X filledLen=%d timeStamp=%Ld"),
		aComponent, aBuffer, aBuffer->nInputPortIndex, aBuffer->nFlags, aBuffer->nFilledLen, aBuffer->nTimeStamp);
#endif
	OMX_ERRORTYPE omxError;
	omxError =  (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		EmptyThisBuffer(aBuffer);
	
	return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::FillThisBuffer(
	OMX_HANDLETYPE aComponent,
	OMX_BUFFERHEADERTYPE* aBuffer)
	{
    DEBUG_PRINTF2(_L8("COmxILComponent::FillThisBuffer : Handle[%X]"), aComponent);
#ifdef _OMXIL_COMMON_BUFFER_TRACING_ON
	RDebug::Print(_L("COmxILComponent::FillThisBuffer component=0x%08X header=0x%08X port=%d"), aComponent, aBuffer, aBuffer->nOutputPortIndex);
#endif
	
	OMX_ERRORTYPE omxError;
	omxError =  (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		FillThisBuffer(aBuffer);
	
	return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::SetCallbacks(
	OMX_HANDLETYPE aComponent,
	OMX_CALLBACKTYPE* aCallbacks,
	OMX_PTR aAppData)
	{
    DEBUG_PRINTF2(_L8("COmxILComponent::SetCallbacks : Handle[%X]"), aComponent);
    OMX_ERRORTYPE omxError;
	omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		SetCallbacks(const_cast<const OMX_CALLBACKTYPE*>(aCallbacks), aAppData);
	return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::ComponentDeInit(
	OMX_HANDLETYPE aComponent)
	{
    DEBUG_PRINTF2(_L8("COmxILComponent::ComponentDeInit : Handle[%X]"), aComponent);
	delete (static_cast<COmxILComponent*>(
				(static_cast<COmxILFsm*>(
					(static_cast<OMX_COMPONENTTYPE*>(aComponent))->
					pComponentPrivate))->GetComponent()));
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
    DEBUG_PRINTF2(_L8("COmxILComponent::UseEGLImage : Handle[%X]"), aComponent);
    OMX_ERRORTYPE omxError;
    omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		UseEGLImage(appBufferHdr, aPortIndex, aAppPrivate, eglImage);
    return omxError;
	}

OMX_ERRORTYPE
COmxILComponent::ComponentRoleEnum(
	OMX_HANDLETYPE aComponent,
	OMX_U8* aRole,
	OMX_U32 aIndex)
	{
    DEBUG_PRINTF2(_L8("COmxILComponent::ComponentRoleEnum : Handle[%X]"), aComponent);
    OMX_ERRORTYPE omxError;
    omxError = (static_cast<COmxILFsm*>
			((static_cast<OMX_COMPONENTTYPE*>(aComponent))->pComponentPrivate))->
		ComponentRoleEnum(aRole, aIndex);
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
