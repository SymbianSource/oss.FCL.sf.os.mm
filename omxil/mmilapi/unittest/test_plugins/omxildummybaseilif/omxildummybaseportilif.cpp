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

#include "omxildummybaseportilif.h"
    
/**
class COmxILDummyBasePortILIF
Concrete implementation of the MILComponentPortIf that encapsulates the 
component's port specific behaviour according to OpenMAX specification.
*/
COmxILDummyBasePortILIF::COmxILDummyBasePortILIF(COmxILDummyBaseILIF& aParent,
										     TPortDirection aPortDirection, 
											 TInt aIndex)
	:
	iPortDirection (aPortDirection),
	iPortIndex (aIndex),						
	iPortComponent(aParent),
	iTunneledPort(NULL)
    {    
    }

COmxILDummyBasePortILIF* COmxILDummyBasePortILIF::NewL(COmxILDummyBaseILIF& aParent, 
                                                       TPortDirection aPortDirection, 
                                                       TInt aIndex)
    {
    COmxILDummyBasePortILIF* self = new (ELeave) COmxILDummyBasePortILIF(aParent, aPortDirection, aIndex);
    return self;
    }
TInt COmxILDummyBasePortILIF::FillThisBuffer(CMMFBuffer& aBuffer)
    { 
    if (iPortDirection == EDirOutput)
        {
		return iPortComponent.OmxFillThisBuffer(&aBuffer, &iPortComponent);			
        }
    else
        {
        return KErrCorrupt;
        }
    }

TInt COmxILDummyBasePortILIF::EmptyThisBuffer(const CMMFBuffer& aBuffer)
    {
    if (iPortDirection == EDirInput)
        {
		return iPortComponent.OmxEmptyThisBuffer(&aBuffer, &iPortComponent);
        }
    else
        {
        return KErrCorrupt;
        }
    }
    
TInt COmxILDummyBasePortILIF::TunnelRequest(MILComponentPortIf* aPort)
    { 
	TInt error = KErrNotSupported;
	
	if (aPort)
		// This is the connet tunnel request
		{
	    TPortDirection otherPortDir = aPort->PortDirection();
	    COmxILDummyBaseILIF* otherPortComponent = static_cast<COmxILDummyBaseILIF*> (aPort->PortComponent());

		if (iPortDirection != otherPortDir)
	        {
			if (iPortDirection == EDirInput)
				{
				error = iPortComponent.OmxComponentTunnelRequest(iPortIndex, 
															 	 otherPortComponent->OmxHandle(), 
															 	 aPort->PortIndex()); 
				}
			else
				{
				error = otherPortComponent->OmxComponentTunnelRequest(aPort->PortIndex(),
														   	 	  	  iPortComponent.OmxHandle(),
																  	  iPortIndex);
				}
	        
			if (error == KErrNone)
				{
				iTunneledPort = static_cast<COmxILDummyBasePortILIF*>(aPort);
				iTunneledPort->SetPortConnectedTo(this);
				}
			}
		}
	else
		// This is the disconnet tunnel request
		{
    
	    TPortDirection otherPortDir = iTunneledPort->PortDirection();
	    COmxILDummyBaseILIF* otherPortComponent = static_cast<COmxILDummyBaseILIF*> (iTunneledPort->PortComponent());
    
		if (iPortDirection != otherPortDir)
			{
			if (iPortDirection == EDirInput)
				{
				error = iPortComponent.OmxComponentDisconnectTunnel(iPortIndex, 
													      			otherPortComponent->OmxHandle(), 
																    iTunneledPort->PortIndex());  
				}
			else
				{
				error = otherPortComponent->OmxComponentDisconnectTunnel(iTunneledPort->PortIndex(),
													   	 	   		     iPortComponent.OmxHandle(),
													   	 	   		     iPortIndex);
				}

			if (error == KErrNone)
				{
				iTunneledPort->SetPortConnectedTo(NULL);
				iTunneledPort = NULL;
				}
			}
		}
	return error;	
    }

TInt COmxILDummyBasePortILIF::PortIndex() const
    {
    return iPortIndex;
    }

TPortDirection COmxILDummyBasePortILIF::PortDirection() const
    {
    return iPortDirection;
    }

CMMFBuffer* COmxILDummyBasePortILIF::CreateBufferL(TInt aBufferSize)
    {
	return iPortComponent.OmxAllocateBufferL(iPortIndex, aBufferSize);	
    }

TInt COmxILDummyBasePortILIF::UseBuffer(CMMFBuffer& aBuffer)
    {
	return iPortComponent.OmxUseBuffer(&aBuffer, iPortIndex);	
    }

TInt COmxILDummyBasePortILIF::FreeBuffer(CMMFBuffer* aBuffer)
    {
	return iPortComponent.OmxFreeBuffer(aBuffer);
    }

TInt COmxILDummyBasePortILIF::FlushPort()
    {
	return iPortComponent.OmxSendCommand(OMX_CommandFlush, iPortIndex, NULL);
    }

TInt COmxILDummyBasePortILIF::EnablePort()
    {
	return iPortComponent.OmxSendCommand(OMX_CommandPortEnable, iPortIndex, NULL);
    }
    
TInt COmxILDummyBasePortILIF::DisablePort()
    {
	return iPortComponent.OmxSendCommand(OMX_CommandPortDisable, iPortIndex, NULL);
    }
    
MILComponentIf* COmxILDummyBasePortILIF::PortComponent() const
    {
	return &iPortComponent;
    }
    

void COmxILDummyBasePortILIF::SetPortConnectedTo(COmxILDummyBasePortILIF* aPort)
	{
	iTunneledPort = aPort;	
	}

