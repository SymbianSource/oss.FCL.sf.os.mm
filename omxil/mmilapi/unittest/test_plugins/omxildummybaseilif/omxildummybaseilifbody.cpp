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

#include "omxildummybaseilifbody.h"
#include "omxilcoreutils.h"
#include "omxilspecversion.h"
#include "milifobserver.h"
#include "tilstructtypes.h"


const TInt KMaxComponentNameLength = 128;


COmxILDummyBaseILIF::CBody::CBody(COmxILDummyBaseILIF& aParent,
								  MILIfObserver& aComponentIfObserver)
	:
	iParent(aParent),
	iIfObserver(aComponentIfObserver)
    {
    }

COmxILDummyBaseILIF::CBody::~CBody()
    {
    iInputPorts.ResetAndDestroy();
    iInputPorts.Close();
    iOutputPorts.ResetAndDestroy();
    iOutputPorts.Close();
    delete iBufferManager;    
    delete iCallbacks;    

	::OMX_FreeHandle(reinterpret_cast<OMX_HANDLETYPE>(iHandle));
    }

COmxILDummyBaseILIF::CBody* COmxILDummyBaseILIF::CBody::NewL(COmxILDummyBaseILIF& aParent,
													 	 MILIfObserver& aComponentIfObserver,
													 	 const TDesC8& aComponentName)
    {
	CBody* self = new (ELeave) CBody(aParent, aComponentIfObserver);
    CleanupStack::PushL(self);
	self->ConstructL(aComponentName);
    CleanupStack::Pop(self);
    return self;
    }
    
void COmxILDummyBaseILIF::CBody::ConstructL(const TDesC8& aComponentName)
    {
	iCallbacks = COmxCallbacks::NewL(iParent);
	
	RBuf8 buf;
	buf.CleanupClosePushL();
	buf.CreateL(aComponentName, KMaxComponentNameLength);
    TUint8* name = const_cast<TUint8*>(buf.PtrZ());

	OMX_ERRORTYPE errorType = ::OMX_GetHandle(reinterpret_cast<OMX_HANDLETYPE*>(&iHandle), 
								reinterpret_cast<OMX_STRING>(name), 
								iCallbacks, 
							    *iCallbacks);
	CleanupStack::PopAndDestroy(&buf);

    User::LeaveIfError(ConvertOmxErrorType(errorType));

    iBufferManager = new (ELeave) COmxBufferManager(iHandle);
    }

OMX_COMPONENTTYPE* COmxILDummyBaseILIF::CBody::Handle() const
    {
    return iHandle;    
    }
    
TInt COmxILDummyBaseILIF::CBody::OmxGetComponentVersion(TPtr8 aComponentName, 
                                                        OMX_VERSIONTYPE* aComponentVersion, 
                                                        OMX_VERSIONTYPE* aSpecVersion, 
                                                        OMX_UUIDTYPE* aComponentUUID)
    {
	ASSERT(aComponentVersion);
	ASSERT(aSpecVersion);
	ASSERT(aComponentUUID);
    TUint8* cname = const_cast<TUint8*>(aComponentName.PtrZ());
    OMX_ERRORTYPE error = iHandle->GetComponentVersion(reinterpret_cast<OMX_HANDLETYPE>(iHandle), 
						  reinterpret_cast<OMX_STRING>(cname), aComponentVersion, aSpecVersion, aComponentUUID);

	TLex8 lex(cname);
	TInt length;
	for( length = 0; !lex.Eos(); ++length )
		{
		lex.Inc();
		}
	aComponentName.SetLength(length);
    return ConvertOmxErrorType(error);
    }
    
TInt COmxILDummyBaseILIF::CBody::OmxSendCommand(OMX_COMMANDTYPE aCmd, 
												TUint32 aParam, 
												TAny* aCmdData)
    {
	OMX_ERRORTYPE error = iHandle->SendCommand(reinterpret_cast<OMX_HANDLETYPE>(iHandle), aCmd, aParam, aCmdData);
    return ConvertOmxErrorType(error);
    }

TInt COmxILDummyBaseILIF::CBody::OmxGetParameter(OMX_INDEXTYPE aParamIndex, 
												 TAny* aValue)
    {
	ASSERT(aValue);
	OMX_ERRORTYPE error = iHandle->GetParameter(reinterpret_cast<OMX_HANDLETYPE>(iHandle), aParamIndex, aValue);
    return ConvertOmxErrorType(error);
    }
    
TInt COmxILDummyBaseILIF::CBody::OmxSetParameter(OMX_INDEXTYPE aIndex, 
												 TAny* aValue)
    {
	ASSERT(aValue);
	OMX_ERRORTYPE error = iHandle->SetParameter(reinterpret_cast<OMX_HANDLETYPE>(iHandle), aIndex, aValue);
    return ConvertOmxErrorType(error);
    }

TInt COmxILDummyBaseILIF::CBody::OmxGetConfig(OMX_INDEXTYPE aIndex, 
											  TAny* aValue)
    {
	ASSERT(aValue);
	OMX_ERRORTYPE error = iHandle->GetConfig(reinterpret_cast<OMX_HANDLETYPE>(iHandle), aIndex, aValue);
    return ConvertOmxErrorType(error);
    }
    
TInt COmxILDummyBaseILIF::CBody::OmxSetConfig(OMX_INDEXTYPE aIndex, 
											  TAny* aValue)
    {
	ASSERT(aValue);
	OMX_ERRORTYPE error = iHandle->SetConfig(reinterpret_cast<OMX_HANDLETYPE>(iHandle), aIndex, aValue);
    return ConvertOmxErrorType(error);
    }
    
TInt COmxILDummyBaseILIF::CBody::OmxGetExtensionIndex(const TDesC8& aParameterName, 
													  OMX_INDEXTYPE* aIndexType)
{
    HBufC8* buf = HBufC8::New(aParameterName.Length()+1);
    if (buf == NULL)
        {
        return KErrNoMemory;
        }
    else
        {
		*buf = aParameterName;
		TUint8* cstring = const_cast<TUint8*>(buf->Des().PtrZ());
		OMX_ERRORTYPE error = iHandle->GetExtensionIndex(reinterpret_cast<OMX_HANDLETYPE>(iHandle), reinterpret_cast<char*>(cstring), aIndexType);
		delete buf;
		return ConvertOmxErrorType(error);
        }
    }

TInt COmxILDummyBaseILIF::CBody::OmxGetState(OMX_STATETYPE*  aState)
    {
	OMX_ERRORTYPE error = iHandle->GetState(reinterpret_cast<OMX_HANDLETYPE>(iHandle), aState);
    return ConvertOmxErrorType(error);
    }
    
TInt COmxILDummyBaseILIF::CBody::OmxComponentTunnelRequest(TUint32 aPortInput, 
														   OMX_HANDLETYPE aOutput, 
														   TUint32 aPortOutput)
    {
	OMX_ERRORTYPE error = ::OMX_SetupTunnel(aOutput, aPortOutput, reinterpret_cast<OMX_HANDLETYPE>(iHandle), aPortInput);
    return ConvertOmxErrorType(error);
    }

TInt COmxILDummyBaseILIF::CBody::OmxComponentDisconnectTunnel(TUint32 aPortInput, 
														 	  OMX_HANDLETYPE aOutput, 
														 	  TUint32 aPortOutput)
	{
	OMX_ERRORTYPE error = ::OMX_SetupTunnel(aOutput, aPortOutput, 0, 0);
	if (error == OMX_ErrorNone)
		{
		error = ::OMX_SetupTunnel(0, 0, reinterpret_cast<OMX_HANDLETYPE>(iHandle), aPortInput);
		}
	return ConvertOmxErrorType(error);
	}
    
TInt COmxILDummyBaseILIF::CBody::OmxUseBuffer(CMMFBuffer* aBuffer, 
											  TUint32 aPortIndex)
    {
	ASSERT(aBuffer);
    return (iBufferManager->UseBuffer(*aBuffer, aPortIndex));    
    }

CMMFBuffer* COmxILDummyBaseILIF::CBody::OmxAllocateBufferL(TUint32 aPortIndex, 
														   TUint32 aSizeBytes)
    {
    return (iBufferManager->AllocateBufferL(aPortIndex, aSizeBytes));
    }

TInt COmxILDummyBaseILIF::CBody::OmxFreeBuffer(CMMFBuffer* aBuffer)
    {
	ASSERT(aBuffer);
    return (iBufferManager->FreeBuffer(aBuffer));
    }

TInt COmxILDummyBaseILIF::CBody::OmxEmptyThisBuffer(const CMMFBuffer* aBuffer, 
													MOmxILComponentIfObserver* aObserver)
    {
	ASSERT(aBuffer);
    return (iBufferManager->EmptyThisBuffer(aBuffer, aObserver));
    }

TInt COmxILDummyBaseILIF::CBody::OmxFillThisBuffer(CMMFBuffer* aBuffer, 
												   MOmxILComponentIfObserver* aObserver)
    {
	ASSERT(aBuffer);
    return (iBufferManager->FillThisBuffer(aBuffer, aObserver));
    }

TInt COmxILDummyBaseILIF::CBody::OmxComponentRoleEnum(TPtr8& aComponentRole, 
													  TUint32 aIndex)
    {
    TUint8* role = const_cast<TUint8*> (aComponentRole.PtrZ());
	OMX_ERRORTYPE error = iHandle->ComponentRoleEnum(reinterpret_cast<OMX_HANDLETYPE>(iHandle), reinterpret_cast<unsigned char*>(role), aIndex);

	TLex8 lex(role);
	TInt length;
	for( length = 0; !lex.Eos(); ++length )
		{
		lex.Inc();
		}
	aComponentRole.SetLength(length);

    return ConvertOmxErrorType(error);
    }

TInt COmxILDummyBaseILIF::CBody::SetIfToRole(const TUid& aComponentRole)
    {
    iFormat = aComponentRole;
    return KErrNone;
    }

TInt COmxILDummyBaseILIF::CBody::GetIfRole(TUid& aComponentRole) const
    {
    aComponentRole = iFormat;
    return KErrNone;    
    }

TInt COmxILDummyBaseILIF::CBody::SetPortsL()
    {
    OMX_PORT_PARAM_TYPE param;
    param.nVersion = KOMXILSpecVersion;
    param.nSize = sizeof(OMX_PORT_PARAM_TYPE);
    TInt err = OmxGetParameter(OMX_IndexParamAudioInit, &param);
    
    if (err == KErrNone)
        {
        for (TInt i=0; i < param.nPorts && err == KErrNone; i++ )
            {
            OMX_PARAM_PORTDEFINITIONTYPE portInfo;
            portInfo.nPortIndex = i;
			portInfo.nVersion = KOMXILSpecVersion;
			portInfo.nSize = sizeof(OMX_PARAM_PORTDEFINITIONTYPE);
            err = OmxGetParameter(OMX_IndexParamPortDefinition, &portInfo);    
			if (err == KErrNone)
				{
            if (portInfo.eDir == OMX_DirInput)
                {
                COmxILDummyBasePortILIF* inputPort = COmxILDummyBasePortILIF::NewL(iParent, EDirInput, portInfo.nPortIndex);
                err = iInputPorts.Append(inputPort);
                }
            else
                {
                COmxILDummyBasePortILIF* outputPort = COmxILDummyBasePortILIF::NewL(iParent, EDirOutput, portInfo.nPortIndex);
                err = iOutputPorts.Append(outputPort);                
					}
				}
			}
		}
    return err;
    }

TInt COmxILDummyBaseILIF::CBody::GetComponentPorts(RPointerArray<MILComponentPortIf>& aComponentPorts, 
												   OMX_DIRTYPE aDirection)
    {
    TInt err = KErrNone;
    if     (aDirection == OMX_DirInput)
        {
		const TInt counter = iInputPorts.Count();
		for (TInt i=0; i < counter && err == KErrNone; i++ )
            {
            err = aComponentPorts.Append(iInputPorts[i]);
            }
        }
    else if (aDirection == OMX_DirOutput)
        {
		const TInt counter = iOutputPorts.Count();
		for (TInt i=0; i < counter && err == KErrNone; i++ )
            {
            err = aComponentPorts.Append(iOutputPorts[i]);
            }            
        }
    else 
        {
        err = KErrNotSupported;
        }
    return err;
    }

TInt COmxILDummyBaseILIF::CBody::FillBufferDone(CMMFBuffer* aBuffer, 
												TInt aPortIndex)
    {
	ASSERT(aBuffer);
    // Loop through all output ports to search for the interface associated to this port index
	const TInt counter = iOutputPorts.Count();
	for (TInt i=0; i < counter; i++ )
        {
        if (iOutputPorts[i]->PortIndex() == aPortIndex)
            {
			return (iIfObserver.BufferDelivered(iOutputPorts[i], aBuffer));
            }
        }

    return KErrNotFound;
    }

TInt COmxILDummyBaseILIF::CBody::EmptyBufferDone(const CMMFBuffer* aBuffer, 
												 TInt aPortIndex)
    {
	ASSERT(aBuffer);
    // Loop through all input ports to search for the interface associated to this port index
	const TInt counter = iInputPorts.Count();
	for (TInt i=0; i < counter; i++ )
        {
        if (iInputPorts[i]->PortIndex() == aPortIndex)
            {
			return (iIfObserver.BufferDelivered(iInputPorts[i], aBuffer));
            }
        }

    return KErrNotFound;
    }

TInt COmxILDummyBaseILIF::CBody::EventHandler(OMX_EVENTTYPE aEvent, 
											  TUint32 aData1, 
											  TUint32 aData2, 
											  TAny* /*aExtraInfo*/)
    {
    TInt err = KErrNone;
    TILEvent thisEvent;

	switch (aEvent)
		{
		case OMX_EventCmdComplete:
			{
			thisEvent.iEvent = EEventCmdComplete;
			if (aData1 == OMX_CommandStateSet)
				{
				thisEvent.iData1 = ECommandStateSet;				
				OMX_STATETYPE state = static_cast<OMX_STATETYPE>(aData2);	
				thisEvent.iData2 = ConvertOmxState(state);
				err = iIfObserver.MsgFromILComponent(&iParent, thisEvent); 
				break;	
				}
			else if (aData1 == OMX_CommandFlush)
				{
				thisEvent.iData1 = ECommandFlush;				
				thisEvent.iData2 = aData2; // Port index
				err = iIfObserver.MsgFromILComponent(&iParent, thisEvent); 
				}
			else if (aData1 == OMX_CommandPortDisable)
				{
				thisEvent.iData1 = ECommandPortDisable;				
				thisEvent.iData2 = aData2; // Port index
				err = iIfObserver.MsgFromILComponent(&iParent, thisEvent); 
				}
			else if (aData1 == OMX_CommandPortEnable)
				{
				thisEvent.iData1 = ECommandPortEnable;				
				thisEvent.iData2 = aData2; // Port index
				err = iIfObserver.MsgFromILComponent(&iParent, thisEvent); 
				}
			else 
				{
				err = KErrNotSupported; 
				}
			
			break;
			}
		case OMX_EventBufferFlag:
			{
			// Propagate the EOF up to client as KErrUnderflow
			thisEvent.iEvent = EEventBufferFlag;
			thisEvent.iData1 = aData1;				
			thisEvent.iData2 = static_cast<TUint32>(KErrUnderflow);
			err = iIfObserver.MsgFromILComponent(&iParent, thisEvent); 
			break;
			}
		case OMX_EventError:
			{
			// Propagate the error up to client
			thisEvent.iEvent = EEventError;
			OMX_ERRORTYPE errorx = static_cast<OMX_ERRORTYPE>(aData1);
			thisEvent.iData1 = ConvertOmxErrorType(errorx);				
			err = iIfObserver.MsgFromILComponent(&iParent, thisEvent); 
			break;
			}
		default:
			{
			err = KErrNotSupported;
			}
		}
	return err;
	}
