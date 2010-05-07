/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef OMX_TRACE_H_
#define OMX_TRACE_H_

#ifdef OMX_DEBUG_TRACING_ON
/* COMPONENT ENTRY POINTS */

#define OMX_TRACE_GETCOMPONENTVERSION_IN(hComponent, pComponentName, pComponentVersion, pSpecVersion, pComponentUUID) RDebug::Printf("DBGOMX_GetComponentVersion %X", hComponent)
//TODO [DL] this is dodgy, what is it fails and then try write pCompnentName as a string
#define OMX_TRACE_GETCOMPONENTVERSION_OUT(hComponent, pComponentName, pComponentVersion, pSpecVersion, pComponentUUID, error) RDebug::Printf("DBGOMX_OUT_GetComponentVersion %X %d", hComponent, error)

#define OMX_TRACE_SENDCOMMAND_IN(hComponent, Cmd, nParam1, pCmdData) RDebug::Printf("DBGOMX_SendCommand %X %d %u", hComponent, Cmd, nParam1)
#define OMX_TRACE_SENDCOMMAND_OUT(hComponent, Cmd, nParam1, pCmdData, error) RDebug::Printf("DBGOMX_OUT_SendCommand %X %d %u %d", hComponent, Cmd, nParam1, error)

#define OMX_TRACE_GETPARAMETER_IN(hComponent, nParamIndex, pComponentParameterStructure) RDebug::Printf("DBGOMX_GetParameter %X %d %X", hComponent, nParamIndex, pComponentParameterStructure)
#define OMX_TRACE_GETPARAMETER_OUT(hComponent, nParamIndex, pComponentParameterStructure, error) RDebug::Printf("DBGOMX_OUT_GetParameter %X %d %X %d",hComponent, nParamIndex, pComponentParameterStructure, error)

#define OMX_TRACE_SETPARAMETER_IN(hComponent, nParamIndex, pComponentParameterStructure) RDebug::Printf("DBGOMX_SetParameter %X %d %X", hComponent, nParamIndex, pComponentParameterStructure)
#define OMX_TRACE_SETPARAMETER_OUT(hComponent, nParamIndex, pComponentParameterStructure, error) RDebug::Printf("DBGOMX_OUT_SetParameter %X %d %X %d", hComponent, nParamIndex, pComponentParameterStructure, error)

#define OMX_TRACE_GETCONFIG_IN(hComponent, nParamIndex, pComponentConfigStructure) RDebug::Printf("DBGOMX_GetConfig %X %d %X",hComponent, nParamIndex, pComponentConfigStructure)
#define OMX_TRACE_GETCONFIG_OUT(hComponent, nParamIndex, pComponentConfigStructure, error) RDebug::Printf("DBGOMX_OUT_GetConfig %X %d %X %d",hComponent, nParamIndex, pComponentConfigStructure, error)

#define OMX_TRACE_SETCONFIG_IN(hComponent, nParamIndex, pComponentConfigStructure) RDebug::Printf("DBGOMX_SetConfig %X %d %X",hComponent, nParamIndex, pComponentConfigStructure)
#define OMX_TRACE_SETCONFIG_OUT(hComponent, nParamIndex, pComponentConfigStructure, error) RDebug::Printf("DBGOMX_OUT_SetConfig %X %d %X %d",hComponent, nParamIndex, pComponentConfigStructure, error)
    
#define OMX_TRACE_GETEXTENSIONINDEX_IN(hComponent, cParameterName, pIndexType) RDebug::Printf("DBGOMX_GetExtensionIndex %X %s", hComponent, cParameterName)
#define OMX_TRACE_GETEXTENSIONINDEX_OUT(hComponent, cParameterName, pIndexType, error) RDebug::Printf("DBGOMX_OUT_GetExtensionIndex %X %s %d",hComponent, cParameterName, error)

#define OMX_TRACE_GETSTATE_IN(hComponent, pState) RDebug::Printf("DBGOMX_GetState %X", hComponent)
#define OMX_TRACE_GETSTATE_OUT(hComponent, pState, error) RDebug::Printf("DBGOMX_OUT_GetState %X %d %d", hComponent, *pState, error)

#define OMX_TRACE_COMPONENTTUNNELREQUEST_IN(hComp, nPort, hTunneledComp, nTunneledPort, pTunnelSetup) RDebug::Printf("DBGOMX_ComponentTunnelRequest %X %u %X %u", hComp, nPort, hTunneledComp, nTunneledPort)
#define OMX_TRACE_COMPONENTTUNNELREQUEST_OUT(hComp, nPort, hTunneledComp, nTunneledPort, pTunnelSetup, error) RDebug::Printf("DBGOMX_OUT_ComponentTunnelRequest %X %u %X %u %d",hComp, nPort, hTunneledComp, nTunneledPort, error)

#define OMX_TRACE_USEBUFFER_IN(hComponent, ppBufferHdr, nPortIndex, pAppPrivate, nSizeBytes, pBuffer) RDebug::Printf("DBGOMX_UseBuffer %X %u %u %X", hComponent, nPortIndex, nSizeBytes, pBuffer )
#define OMX_TRACE_USEBUFFER_OUT(hComponent, ppBufferHdr, nPortIndex, pAppPrivate, nSizeBytes, pBuffer, error) RDebug::Printf("DBGOMX_OUT_UseBuffer %X %X %u %u %X %d", hComponent, *ppBufferHdr, nPortIndex, nSizeBytes, pBuffer, error)

#define OMX_TRACE_ALLOCATEBUFFER_IN(hComponent, ppBuffer, nPortIndex, pAppPrivate, nSizeBytes) RDebug::Printf("DBGOMX_AllocateBuffer %X %u %u", hComponent, nPortIndex, nSizeBytes)
#define OMX_TRACE_ALLOCATEBUFFER_OUT(hComponent, ppBuffer, nPortIndex, pAppPrivate, nSizeBytes, error) RDebug::Printf("DBGOMX_OUT_AllocateBuffer %X %X %u %u %d", hComponent, *ppBuffer, nPortIndex, nSizeBytes, error)

#define OMX_TRACE_FREEBUFFER_IN(hComponent, nPortIndex, pBuffer) RDebug::Printf("DBGOMX_FreeBuffer %X %u %X", hComponent, nPortIndex, pBuffer)
#define OMX_TRACE_FREEBUFFER_OUT(hComponent, nPortIndex, pBuffer, error) RDebug::Printf("DBGOMX_OUT_FreeBuffer %X %u %X %d", hComponent, nPortIndex, pBuffer, error)

#define OMX_TRACE_EMPTYTHISBUFFER_IN(hComponent, pBuffer) RDebug::Printf("DBGOMX OMX_EmptyThisBuffer %X %X %u %u %u %X %u %Lu %u %u %u 0", hComponent, pBuffer, pBuffer->nAllocLen, pBuffer->nFilledLen, pBuffer->nOffset, pBuffer->hMarkTargetComponent, pBuffer->nTickCount, pBuffer->nTimeStamp, pBuffer->nFlags, pBuffer->nOutputPortIndex, pBuffer->nInputPortIndex)
#define OMX_TRACE_EMPTYTHISBUFFER_OUT(hComponent, pBuffer, error) RDebug::Printf("DBGOMX_OUT_EmptyThisBuffer %X %X %d", hComponent, pBuffer, error)

#define OMX_TRACE_FILLTHISBUFFER_IN(hComponent, pBuffer) RDebug::Printf("DBGOMX OMX_FillThisBuffer %X %X %u %u %u %X %u %Lu %u %u %u 0", hComponent, pBuffer, pBuffer->nAllocLen, pBuffer->nFilledLen, pBuffer->nOffset, pBuffer->hMarkTargetComponent, pBuffer->nTickCount, pBuffer->nTimeStamp, pBuffer->nFlags, pBuffer->nOutputPortIndex, pBuffer->nInputPortIndex)
#define OMX_TRACE_FILLTHISBUFFER_OUT(hComponent, pBuffer, error) RDebug::Printf("DBGOMX_OUT_FillThisBuffer %X %X %d", hComponent, pBuffer, error)

#define OMX_TRACE_SETCALLBACKS_IN(hComponent, pCallbacks, pAppData) RDebug::Printf("DBGOMX_SetCallBacks %X %X", hComponent, pCallbacks)
#define OMX_TRACE_SETCALLBACKS_OUT(hComponent, pCallbacks, pAppData, error) RDebug::Printf("DBGOMX_OUT_SetCallBacks %X %X %d", hComponent, pCallbacks, error)

#define OMX_TRACE_COMPONENTDEINIT_IN(hComponent) RDebug::Printf("DBGOMX_ComponentDeinit %X", hComponent )
#define OMX_TRACE_COMPONENTDEINIT_OUT(hComponent, error) RDebug::Printf("DBGOMX_OUT_ComponentDeinit %X %d", hComponent, error)

#define OMX_TRACE_USEEGLIMAGE_IN(hComponent, ppBufferHdr, nPortIndex, pAppPrivate, eglImage) RDebug::Printf("DBGOMX_UseGLImage %X", hComponent)
#define OMX_TRACE_USEEGLIMAGE_OUT(hComponent, ppBufferHdr, nPortIndex, pAppPrivate, eglImage, error) RDebug::Printf("DBGOMX_OUT_UseGLImage %X %d", hComponent, error)

#define OMX_TRACE_COMPONENTROLEENUM_IN(hComponent, cRole, nIndex) RDebug::Printf("DBGOMX_ComponentRoleEnum %X %s %u", hComponent, cRole, nIndex )
#define OMX_TRACE_COMPONENTROLEENUM_OUT(hComponent, cRole, nIndex, error) RDebug::Printf("DBGOMX_OUT_ComponentRoleEnum %X %s %u %d", hComponent, cRole, nIndex, error)


/* IL CLIENT CALLBACKS */


#define OMX_TRACE_EVENTHANDLER_IN(hComponent, pAppData, eEvent, nData1, nData2, pEventData) RDebug::Printf("DBGOMX_EventHandler %X %d %u %u", hComponent, eEvent, nData1, nData2)
#define OMX_TRACE_EVENTHANDLER_OUT(hComponent, pAppData, eEvent, nData1, nData2, pEventData, error) RDebug::Printf("DBGOMX_OUT_EventHandler %X %d %u %u  %d", hComponent, eEvent, nData1, nData2, error)

#define OMX_TRACE_EMPTYBUFFERDONE_IN(hComponent, pAppData, pBuffer) RDebug::Printf("DBGOMX_EmptyBufferDone %X %X", hComponent, pBuffer )
#define OMX_TRACE_EMPTYBUFFERDONE_OUT(hComponent, pAppData, pBuffer, error) RDebug::Printf("DBGOMX_OUT_EmptyBufferDone %X %X %d", hComponent, pBuffer, error)

#define OMX_TRACE_FILLBUFFERDONE_IN(hComponent, pAppData, pBuffer) RDebug::Printf("DBGOMX_FillBufferDone %X %X", hComponent, pBuffer)
#define OMX_TRACE_FILLBUFFERDONE_OUT(hComponent, pAppData, pBuffer, error) RDebug::Printf("DBGOMX_OUT_FillBufferDone %X %X %d", hComponent, pBuffer, error)


/* CORE METHODS */


#define OMX_TRACE_INIT_IN() RDebug::Printf("DBGOMX_Init")
#define OMX_TRACE_INIT_OUT(error) RDebug::Printf("DBGOMX_OUT_Init %d", error)

#define OMX_TRACE_DEINIT_IN() RDebug::Printf("DBGOMX_Deinit")
#define OMX_TRACE_DEINIT_OUT(error) RDebug::Printf("DBGOMX_OUT_Deinit %d", error)

#define OMX_TRACE_COMPONENTNAMEENUM_IN(cComponentName, nNameLength, nIndex) RDebug::Printf("DBGOMX_ComponentNameEnum %u %u", nNameLength, nIndex )
#define OMX_TRACE_COMPONENTNAMEENUM_OUT(cComponentName, nNameLength, nIndex, error) RDebug::Printf("DBGOMX_OUT_ComponentNameEnum %u %u %d", nNameLength, nIndex, error)

#define OMX_TRACE_GETHANDLE_IN(pHandle, cComponentName, pAppData, pCallBacks)  RDebug::Printf("DBGOMX_GetHandle %s", cComponentName) 
#define OMX_TRACE_GETHANDLE_OUT(pHandle, cComponentName, pAppData, pCallBacks, error) RDebug::Printf("DBGOMX_GetHandle %s %X %d", cComponentName, pHandle, error)

#define OMX_TRACE_FREEHANDLE_IN(hComponent) RDebug::Printf("DBGOMX_FreeHandle %X 0", hComponent)
#define OMX_TRACE_FREEHANDLE_OUT(hComponent, error) RDebug::Printf("DBGOMX_OUT_FreeHandle %X %d", hComponent, error)

#define OMX_TRACE_SETUPTUNNEL_IN(hOutput, nPortOutput, hInput, nPortInput) RDebug::Printf("DBGOMX_SetupTunnel %X %d %X %d 0", hOutput, nPortOutput, hInput, nPortInput)
#define OMX_TRACE_SETUPTUNNEL_OUT(hOutput, nPortOutput, hInput, nPortInput, error) RDebug::Printf("DBGOMX_OUT_SetupTunnel %X %d %X %d %d", hOutput, nPortOutput, hInput, nPortInput, error)

#define OMX_TRACE_GETCONTENTPIPE_IN(hPipe, szURI) RDebug::Printf("DBGOMX_GetContentPipe")
#define OMX_TRACE_GETCONTENTPIPE_OUT(hPipe, szURI, error) RDebug::Printf("DBGOMX_OUT_GetConentPipe %d", error)

#define OMX_TRACE_GETCOMPONENTSOFROLE_IN(role, pNumComps, compNames) RDebug::Printf("DBGOMX_GetComponentsOfRole %s", role)
#define OMX_TRACE_GETCOMPONENTSOFROLE_OUT(role, pNumComps, compNames, error) RDebug::Printf("DBGOMX_OUT_GetComponentsOfRole %s %d", role, error)

#define OMX_TRACE_GETROLESOFCOMPONENT_IN(compName, pNumRoles, roles) RDebug::Printf("DBGOMX_GetRolesOfComponent %s", compName)
#define OMX_TRACE_GETROLESOFCOMPONENT_OUT(compName, pNumRoles, roles, error) RDebug::Printf("DBGOMX_OUT_GetRolesOfComponent %s %d", compName, error)

#elif defined(SYMBIAN_MULTIMEDIA_TURN_TIMING_TRACING_ON)

#include <mmvideoperformance.h>

/* COMPONENT ENTRY POINTS */

#define OMX_TRACE_GETCOMPONENTVERSION_IN(hComponent, pComponentName, pComponentVersion, pSpecVersion, pComponentUUID)
#define OMX_TRACE_GETCOMPONENTVERSION_OUT(hComponent, pComponentName, pComponentVersion, pSpecVersion, pComponentUUID, error)

#define OMX_TRACE_SENDCOMMAND_IN(hComponent, Cmd, nParam1, pCmdData)
#define OMX_TRACE_SENDCOMMAND_OUT(hComponent, Cmd, nParam1, pCmdData, error)

#define OMX_TRACE_GETPARAMETER_IN(hComponent, nParamIndex, pComponentParameterStructure)
#define OMX_TRACE_GETPARAMETER_OUT(hComponent, nParamIndex, pComponentParameterStructure, error)

#define OMX_TRACE_SETPARAMETER_IN(hComponent, nParamIndex, pComponentParameterStructure)
#define OMX_TRACE_SETPARAMETER_OUT(hComponent, nParamIndex, pComponentParameterStructure, error)

#define OMX_TRACE_GETCONFIG_IN(hComponent, nParamIndex, pComponentConfigStructure)
#define OMX_TRACE_GETCONFIG_OUT(hComponent, nParamIndex, pComponentConfigStructure, error)

#define OMX_TRACE_SETCONFIG_IN(hComponent, nParamIndex, pComponentConfigStructure)
#define OMX_TRACE_SETCONFIG_OUT(hComponent, nParamIndex, pComponentConfigStructure, error)
	
#define OMX_TRACE_GETEXTENSIONINDEX_IN(hComponent, cParameterName, pIndexType)
#define OMX_TRACE_GETEXTENSIONINDEX_OUT(hComponent, cParameterName, pIndexType, error)

#define OMX_TRACE_GETSTATE_IN(hComponent, pState)
#define OMX_TRACE_GETSTATE_OUT(hComponent, pState, error)

#define OMX_TRACE_COMPONENTTUNNELREQUEST_IN(hComp, nPort, hTunneledComp, nTunneledPort, pTunnelSetup)
#define OMX_TRACE_COMPONENTTUNNELREQUEST_OUT(hComp, nPort, hTunneledComp, nTunneledPort, pTunnelSetup, error)

#define OMX_TRACE_USEBUFFER_IN(hComponent, ppBufferHdr, nPortIndex, pAppPrivate, nSizeBytes, pBuffer)
#define OMX_TRACE_USEBUFFER_OUT(hComponent, ppBufferHdr, nPortIndex, pAppPrivate, nSizeBytes, pBuffer, error)

#define OMX_TRACE_ALLOCATEBUFFER_IN(hComponent, ppBuffer, nPortIndex, pAppPrivate, nSizeBytes)
#define OMX_TRACE_ALLOCATEBUFFER_OUT(hComponent, ppBuffer, nPortIndex, pAppPrivate, nSizeBytes, error)

#define OMX_TRACE_FREEBUFFER_IN(hComponent, nPortIndex, pBuffer)
#define OMX_TRACE_FREEBUFFER_OUT(hComponent, nPortIndex, pBuffer, error)

#define OMX_TRACE_EMPTYTHISBUFFER_IN(hComponent, pBuffer)
#define OMX_TRACE_EMPTYTHISBUFFER_OUT(hComponent, pBuffer, error)

#define OMX_TRACE_FILLTHISBUFFER_IN(hComponent, pBuffer)
#define OMX_TRACE_FILLTHISBUFFER_OUT(hComponent, pBuffer, error)

#define OMX_TRACE_SETCALLBACKS_IN(hComponent, pCallbacks, pAppData)
#define OMX_TRACE_SETCALLBACKS_OUT(hComponent, pCallbacks, pAppData, error)

#define OMX_TRACE_COMPONENTDEINIT_IN(hComponent)
#define OMX_TRACE_COMPONENTDEINIT_OUT(hComponent, error)

#define OMX_TRACE_USEEGLIMAGE_IN(hComponent, ppBufferHdr, nPortIndex, pAppPrivate, eglImage)
#define OMX_TRACE_USEEGLIMAGE_OUT(hComponent, ppBufferHdr, nPortIndex, pAppPrivate, eglImage, error)

#define OMX_TRACE_COMPONENTROLEENUM_IN(hComponent, cRole, nIndex)
#define OMX_TRACE_COMPONENTROLEENUM_OUT(hComponent, cRole, nIndex, error)


/* IL CLIENT CALLBACKS */


#define OMX_TRACE_EVENTHANDLER_IN(hComponent, pAppData, eEvent, nData1, nData2, pEventData)
#define OMX_TRACE_EVENTHANDLER_OUT(hComponent, pAppData, eEvent, nData1, nData2, pEventData, error)

#define OMX_TRACE_EMPTYBUFFERDONE_IN(hComponent, pAppData, pBuffer)
#define OMX_TRACE_EMPTYBUFFERDONE_OUT(hComponent, pAppData, pBuffer, error)

#define OMX_TRACE_FILLBUFFERDONE_IN(hComponent, pAppData, pBuffer)
#define OMX_TRACE_FILLBUFFERDONE_OUT(hComponent, pAppData, pBuffer, error)


/* CORE METHODS */


#define OMX_TRACE_INIT_IN()
#define OMX_TRACE_INIT_OUT(error)

#define OMX_TRACE_DEINIT_IN()
#define OMX_TRACE_DEINIT_OUT(error)

#define OMX_TRACE_COMPONENTNAMEENUM_IN(cComponentName, nNameLength, nIndex)
#define OMX_TRACE_COMPONENTNAMEENUM_OUT(cComponentName, nNameLength, nIndex, error)

#define OMX_TRACE_GETHANDLE_IN(pHandle, cComponentName, pAppData, pCallBacks) TUTrace::PrintfPrimary(KTestTraceFilterId, 0, 0, "VPT %x", ETestTraceIdOmxPerformanceTraceGetHandleIn)
#define OMX_TRACE_GETHANDLE_OUT(pHandle, cComponentName, pAppData, pCallBacks, error) TUTrace::PrintfPrimary(KTestTraceFilterId, 0, 0, "VPT %x %x %s", ETestTraceIdOmxPerformanceTraceGetHandleOut, pHandle, cComponentName)

#define OMX_TRACE_FREEHANDLE_IN(hComponent)
#define OMX_TRACE_FREEHANDLE_OUT(hComponent, error)

#define OMX_TRACE_SETUPTUNNEL_IN(hOutput, nPortOutput, hInput, nPortInput) TUTrace::PrintfPrimary(KTestTraceFilterId, 0, 0, "VPT %x %x %x %x %x", ETestTraceIdOmxPerformanceTraceSetupTunnelIn, hOutput, nPortOutput, hInput, nPortInput)
#define OMX_TRACE_SETUPTUNNEL_OUT(hOutput, nPortOutput, hInput, nPortInput, error) TUTrace::PrintfPrimary(KTestTraceFilterId, 0, 0, "VPT %x", ETestTraceIdOmxPerformanceTraceSetupTunnelOut)

#define OMX_TRACE_GETCONTENTPIPE_IN(hPipe, szURI)
#define OMX_TRACE_GETCONTENTPIPE_OUT(hPipe, szURI, error)

#define OMX_TRACE_GETCOMPONENTSOFROLE_IN(role, pNumComps, compNames)
#define OMX_TRACE_GETCOMPONENTSOFROLE_OUT(role, pNumComps, compNames, error)

#define OMX_TRACE_GETROLESOFCOMPONENT_IN(compName, pNumRoles, roles)
#define OMX_TRACE_GETROLESOFCOMPONENT_OUT(compName, pNumRoles, roles, error)

#else

/* COMPONENT ENTRY POINTS */

#define OMX_TRACE_GETCOMPONENTVERSION_IN(hComponent, pComponentName, pComponentVersion, pSpecVersion, pComponentUUID)
#define OMX_TRACE_GETCOMPONENTVERSION_OUT(hComponent, pComponentName, pComponentVersion, pSpecVersion, pComponentUUID, error)

#define OMX_TRACE_SENDCOMMAND_IN(hComponent, Cmd, nParam1, pCmdData)
#define OMX_TRACE_SENDCOMMAND_OUT(hComponent, Cmd, nParam1, pCmdData, error)

#define OMX_TRACE_GETPARAMETER_IN(hComponent, nParamIndex, pComponentParameterStructure)
#define OMX_TRACE_GETPARAMETER_OUT(hComponent, nParamIndex, pComponentParameterStructure, error)

#define OMX_TRACE_SETPARAMETER_IN(hComponent, nParamIndex, pComponentParameterStructure)
#define OMX_TRACE_SETPARAMETER_OUT(hComponent, nParamIndex, pComponentParameterStructure, error)

#define OMX_TRACE_GETCONFIG_IN(hComponent, nParamIndex, pComponentConfigStructure)
#define OMX_TRACE_GETCONFIG_OUT(hComponent, nParamIndex, pComponentConfigStructure, error)

#define OMX_TRACE_SETCONFIG_IN(hComponent, nParamIndex, pComponentConfigStructure)
#define OMX_TRACE_SETCONFIG_OUT(hComponent, nParamIndex, pComponentConfigStructure, error)
    
#define OMX_TRACE_GETEXTENSIONINDEX_IN(hComponent, cParameterName, pIndexType)
#define OMX_TRACE_GETEXTENSIONINDEX_OUT(hComponent, cParameterName, pIndexType, error)

#define OMX_TRACE_GETSTATE_IN(hComponent, pState)
#define OMX_TRACE_GETSTATE_OUT(hComponent, pState, error)

#define OMX_TRACE_COMPONENTTUNNELREQUEST_IN(hComp, nPort, hTunneledComp, nTunneledPort, pTunnelSetup)
#define OMX_TRACE_COMPONENTTUNNELREQUEST_OUT(hComp, nPort, hTunneledComp, nTunneledPort, pTunnelSetup, error)

#define OMX_TRACE_USEBUFFER_IN(hComponent, ppBufferHdr, nPortIndex, pAppPrivate, nSizeBytes, pBuffer)
#define OMX_TRACE_USEBUFFER_OUT(hComponent, ppBufferHdr, nPortIndex, pAppPrivate, nSizeBytes, pBuffer, error)

#define OMX_TRACE_ALLOCATEBUFFER_IN(hComponent, ppBuffer, nPortIndex, pAppPrivate, nSizeBytes)
#define OMX_TRACE_ALLOCATEBUFFER_OUT(hComponent, ppBuffer, nPortIndex, pAppPrivate, nSizeBytes, error)

#define OMX_TRACE_FREEBUFFER_IN(hComponent, nPortIndex, pBuffer)
#define OMX_TRACE_FREEBUFFER_OUT(hComponent, nPortIndex, pBuffer, error)

#define OMX_TRACE_EMPTYTHISBUFFER_IN(hComponent, pBuffer)
#define OMX_TRACE_EMPTYTHISBUFFER_OUT(hComponent, pBuffer, error)

#define OMX_TRACE_FILLTHISBUFFER_IN(hComponent, pBuffer)
#define OMX_TRACE_FILLTHISBUFFER_OUT(hComponent, pBuffer, error)

#define OMX_TRACE_SETCALLBACKS_IN(hComponent, pCallbacks, pAppData)
#define OMX_TRACE_SETCALLBACKS_OUT(hComponent, pCallbacks, pAppData, error)

#define OMX_TRACE_COMPONENTDEINIT_IN(hComponent)
#define OMX_TRACE_COMPONENTDEINIT_OUT(hComponent, error)

#define OMX_TRACE_USEEGLIMAGE_IN(hComponent, ppBufferHdr, nPortIndex, pAppPrivate, eglImage)
#define OMX_TRACE_USEEGLIMAGE_OUT(hComponent, ppBufferHdr, nPortIndex, pAppPrivate, eglImage, error)

#define OMX_TRACE_COMPONENTROLEENUM_IN(hComponent, cRole, nIndex)
#define OMX_TRACE_COMPONENTROLEENUM_OUT(hComponent, cRole, nIndex, error)


/* IL CLIENT CALLBACKS */


#define OMX_TRACE_EVENTHANDLER_IN(hComponent, pAppData, eEvent, nData1, nData2, pEventData)
#define OMX_TRACE_EVENTHANDLER_OUT(hComponent, pAppData, eEvent, nData1, nData2, pEventData, error)

#define OMX_TRACE_EMPTYBUFFERDONE_IN(hComponent, pAppData, pBuffer)
#define OMX_TRACE_EMPTYBUFFERDONE_OUT(hComponent, pAppData, pBuffer, error)

#define OMX_TRACE_FILLBUFFERDONE_IN(hComponent, pAppData, pBuffer)
#define OMX_TRACE_FILLBUFFERDONE_OUT(hComponent, pAppData, pBuffer, error)


/* CORE METHODS */


#define OMX_TRACE_INIT_IN()
#define OMX_TRACE_INIT_OUT(error)

#define OMX_TRACE_DEINIT_IN()
#define OMX_TRACE_DEINIT_OUT(error)

#define OMX_TRACE_COMPONENTNAMEENUM_IN(cComponentName, nNameLength, nIndex)
#define OMX_TRACE_COMPONENTNAMEENUM_OUT(cComponentName, nNameLength, nIndex, error)

#define OMX_TRACE_GETHANDLE_IN(pHandle, cComponentName, pAppData, pCallBacks)
#define OMX_TRACE_GETHANDLE_OUT(pHandle, cComponentName, pAppData, pCallBacks, error)

#define OMX_TRACE_FREEHANDLE_IN(hComponent)
#define OMX_TRACE_FREEHANDLE_OUT(hComponent, error)

#define OMX_TRACE_SETUPTUNNEL_IN(hOutput, nPortOutput, hInput, nPortInput)
#define OMX_TRACE_SETUPTUNNEL_OUT(hOutput, nPortOutput, hInput, nPortInput, error)

#define OMX_TRACE_GETCONTENTPIPE_IN(hPipe, szURI)
#define OMX_TRACE_GETCONTENTPIPE_OUT(hPipe, szURI, error)

#define OMX_TRACE_GETCOMPONENTSOFROLE_IN(role, pNumComps, compNames)
#define OMX_TRACE_GETCOMPONENTSOFROLE_OUT(role, pNumComps, compNames, error)

#define OMX_TRACE_GETROLESOFCOMPONENT_IN(compName, pNumRoles, roles)
#define OMX_TRACE_GETROLESOFCOMPONENT_OUT(compName, pNumRoles, roles, error)

#endif

#endif // OMX_TRACE_H_
