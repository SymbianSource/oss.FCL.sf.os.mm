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


#include "omxildummybaseilif.h"
#include "omxildummybaseilifbody.h"
#include "omxilcoreutils.h" 
#include "iltestuids.hrh"
#include "milcomponentportif.h"
#include "ilifuids.h"
#include <openmax/il/khronos/v1_x/OMX_Types.h>

const TUint32  KOMXALL = 0xFFFFFFFF;


EXPORT_C COmxILDummyBaseILIF::COmxILDummyBaseILIF()
    { 
    }

EXPORT_C COmxILDummyBaseILIF::~COmxILDummyBaseILIF()
    {
	if (iBody)
		{
		delete iBody;
		}
    }

EXPORT_C void COmxILDummyBaseILIF::CreateComponentL(const TDesC8& aComponentName, 
                                                    MILIfObserver& aComponentIfObserver)
    {
	iBody = CBody::NewL(*this, aComponentIfObserver, aComponentName);

    User::LeaveIfError(iBody->SetPortsL());
    }

EXPORT_C void COmxILDummyBaseILIF::CreateComponentL(const TUid& /*aUid*/, 
												    MILIfObserver& /*ComponentIfObserver*/)
    {
    // OMX Versions do not use this method of creation
	User::Leave(KErrNotSupported);
	}

EXPORT_C void COmxILDummyBaseILIF::ReleaseComponent()
    {
 	delete iBody;
 	iBody = NULL;
 	
 	delete this;
    }

EXPORT_C TInt COmxILDummyBaseILIF::GetComponentInputPorts(RPointerArray<MILComponentPortIf>& aComponentPorts) const
    {
    return (iBody->GetComponentPorts(aComponentPorts, OMX_DirInput));
    }

EXPORT_C TInt COmxILDummyBaseILIF::GetComponentOutputPorts(RPointerArray<MILComponentPortIf>& aComponentPorts) const
    {
    return (iBody->GetComponentPorts(aComponentPorts, OMX_DirOutput));    
    }

EXPORT_C TInt COmxILDummyBaseILIF::SetConfig(const TILStruct& /*aConfig*/, 
                                             const MILComponentPortIf* /*aComponentPort*/)
    {
    // It should be implemented in the derived class
    return KErrNotSupported;
    }

EXPORT_C TInt COmxILDummyBaseILIF::GetConfig(TILStruct& /*aConfig*/, 
                                             const MILComponentPortIf* /*aComponentPort*/) const
    {
    // It should be implemented in the derived class
	return (KErrNotSupported);
    }

EXPORT_C TInt COmxILDummyBaseILIF::Initialize()
    {
    return (iBody->OmxSendCommand(OMX_CommandStateSet, OMX_StateIdle, NULL));
    }

EXPORT_C TInt COmxILDummyBaseILIF::Execute()
    {
    return (iBody->OmxSendCommand(OMX_CommandStateSet, OMX_StateExecuting, NULL));
    } 

EXPORT_C TInt COmxILDummyBaseILIF::Pause()
    {
    return (iBody->OmxSendCommand(OMX_CommandStateSet, OMX_StatePause, NULL));
    }
    
EXPORT_C TInt COmxILDummyBaseILIF::Stop()
    {
    return (iBody->OmxSendCommand(OMX_CommandStateSet, OMX_StateIdle, NULL));    
    }
    
EXPORT_C TInt COmxILDummyBaseILIF::UnInitialize()
	{
	return (iBody->OmxSendCommand(OMX_CommandStateSet, OMX_StateLoaded, NULL));	
	}

EXPORT_C TInt COmxILDummyBaseILIF::GetState(TILComponentState& aState) const
    {
    TInt error; 
    OMX_STATETYPE state;
    error = iBody->OmxGetState(&state);
    aState = ConvertOmxState(state);
    return error;
    }

EXPORT_C TInt COmxILDummyBaseILIF::SendCommand(const TILCommand& aCommand)
    {
    OMX_COMMANDTYPE command = ConvertSymbianCommandType(aCommand.iCmd);
    return (iBody->OmxSendCommand(command, aCommand.iData1, aCommand.iExtraData));
    }

EXPORT_C TInt COmxILDummyBaseILIF::FlushAllPorts()
    {
    return (iBody->OmxSendCommand(OMX_CommandFlush, KOMXALL, NULL));
    }

EXPORT_C TInt COmxILDummyBaseILIF::ComponentRoleEnum(TPtr8& aComponentRole, 
                                                     TUint32 aIndex) const
    {
    return (iBody->OmxComponentRoleEnum(aComponentRole, aIndex));
    }

EXPORT_C TInt COmxILDummyBaseILIF::GetComponentVersion(TILComponentVersion& aVersion) const
    {
    return (iBody->OmxGetComponentVersion(aVersion.iComponentName, 
                                          (OMX_VERSIONTYPE*)&aVersion.iComponentVersion, 
                                          (OMX_VERSIONTYPE*)&aVersion.iSpecVersion, 
                                          (OMX_UUIDTYPE*)&aVersion.iComponentUniqueID));    
    }

EXPORT_C TInt COmxILDummyBaseILIF::SetComponentIfRole(const TUid& aComponentRole)
    {
    return (iBody->SetIfToRole(aComponentRole));
    }
    
EXPORT_C TInt COmxILDummyBaseILIF::SetComponentIfRole(const TDesC8& /*aComponentRole*/)
    {
    return (KErrNotSupported);
    }

EXPORT_C TInt COmxILDummyBaseILIF::GetComponentIfRole(TUid& aComponentRole) const
    {
    return (iBody->GetIfRole(aComponentRole));
    }

EXPORT_C TInt COmxILDummyBaseILIF::GetComponentIfRole(TDes8& /*aComponentRole*/) const
    {
    return (KErrNotSupported);
    }

EXPORT_C TInt COmxILDummyBaseILIF::SetExtConfig(const TDesC8& /*aParameterName*/, 
                                                const TILStruct& /*aConfig*/, 
                                                const MILComponentPortIf* /*aComponentPort*/)
    {
    // It should be implemented in the derived class
	return (KErrNotSupported);
    }

EXPORT_C TInt COmxILDummyBaseILIF::GetExtConfig(const TDesC8& /*aParameterName*/, 
                                                TILStruct& /*aConfig*/, 
                                                const MILComponentPortIf* /*aComponentPort*/) const
    {
    // It should be implemented in the derived class
	return (KErrNotSupported);
    }
EXPORT_C TAny* COmxILDummyBaseILIF::CustomInterface(TUid aUid)
    {
    if (aUid == KUidILComponentIf)
        {
        MILComponentIf* componentIf = this;
        return componentIf;
        }
    else        
        {
        return NULL;
        }
    }


EXPORT_C TInt COmxILDummyBaseILIF::FillBufferDone(CMMFBuffer* aBuffer, 
                                                  TInt aPortIndex)
    {
    return (iBody->FillBufferDone(aBuffer, aPortIndex));
    }

EXPORT_C TInt COmxILDummyBaseILIF::EmptyBufferDone(const CMMFBuffer* aBuffer, 
                                                   TInt aPortIndex)
    {
    return (iBody->EmptyBufferDone(aBuffer, aPortIndex));
    }

EXPORT_C TInt COmxILDummyBaseILIF::EventHandler(OMX_EVENTTYPE aEvent, 
                                                TUint32 aData1, 
                                                TUint32 aData2, 
                                                TAny* aExtraInfo)
    {
    return (iBody->EventHandler(aEvent, aData1, aData2, aExtraInfo));
    }

EXPORT_C TInt COmxILDummyBaseILIF::OmxGetParameter(OMX_INDEXTYPE aParamIndex, 
                                                   TAny* aComponentParameterStructure) const
    {
    return (iBody->OmxGetParameter(aParamIndex, aComponentParameterStructure));
    }

EXPORT_C TInt COmxILDummyBaseILIF::OmxSetParameter(OMX_INDEXTYPE aParamIndex, 
                                                   TAny* aComponentParameterStructure)
    {
    return (iBody->OmxSetParameter(aParamIndex, aComponentParameterStructure));
    }

EXPORT_C TInt COmxILDummyBaseILIF::OmxGetConfig(OMX_INDEXTYPE aConfigIndex, 
                                                TAny* aValue) const
    {
    return (iBody->OmxGetConfig(aConfigIndex, aValue));
    }

EXPORT_C TInt COmxILDummyBaseILIF::OmxSetConfig(OMX_INDEXTYPE aConfigIndex, 
                                                TAny* aValue)
    {
    return (iBody->OmxSetConfig(aConfigIndex, aValue));
    }

EXPORT_C TInt COmxILDummyBaseILIF::OmxGetExtensionIndex(const TDesC8& aParameterName, 
                                                        OMX_INDEXTYPE* aIndexType) const
    {
    return (iBody->OmxGetExtensionIndex(aParameterName, aIndexType));
    }

TInt COmxILDummyBaseILIF::OmxSendCommand(OMX_COMMANDTYPE aCmd, 
                                         TUint32 aParam1, 
                                         TAny* aCmdData)
    {
    return (iBody->OmxSendCommand(aCmd, aParam1, aCmdData));
    }

TInt COmxILDummyBaseILIF::OmxComponentTunnelRequest(TUint32 aPortInput, 
                                                    OMX_HANDLETYPE aOutput, 
                                                    TUint32 aPortOutput)
    {
    return (iBody->OmxComponentTunnelRequest(aPortInput, aOutput, aPortOutput));
    }

TInt COmxILDummyBaseILIF::OmxComponentDisconnectTunnel(TUint32 aPortInput, 
												    OMX_HANDLETYPE aOutput, 
												    TUint32 aPortOutput)
	{
	return (iBody->OmxComponentDisconnectTunnel(aPortInput, aOutput, aPortOutput));
	}

TInt COmxILDummyBaseILIF::OmxUseBuffer(CMMFBuffer* aBuffer, 
                                       TUint32 aPortIndex)
    {
    return (iBody->OmxUseBuffer(aBuffer, aPortIndex));
    }

CMMFBuffer* COmxILDummyBaseILIF::OmxAllocateBufferL(TUint32 aPortIndex, 
                                                    TUint32 aSizeBytes)
    {
    return (iBody->OmxAllocateBufferL(aPortIndex, aSizeBytes));
    }

TInt COmxILDummyBaseILIF::OmxFreeBuffer(CMMFBuffer* aBuffer)
    {
    return (iBody->OmxFreeBuffer(aBuffer));
    }

TInt COmxILDummyBaseILIF::OmxEmptyThisBuffer(const CMMFBuffer* aBuffer, 
                                             MOmxILComponentIfObserver* aObserver)
    {
    return (iBody->OmxEmptyThisBuffer(aBuffer, aObserver));
    }

TInt COmxILDummyBaseILIF::OmxFillThisBuffer(CMMFBuffer* aBuffer, 
                                            MOmxILComponentIfObserver* aObserver)
    {
    return (iBody->OmxFillThisBuffer(aBuffer, aObserver));
    }

OMX_COMPONENTTYPE* COmxILDummyBaseILIF::OmxHandle() const
    {
    return (iBody->Handle());    
    }
