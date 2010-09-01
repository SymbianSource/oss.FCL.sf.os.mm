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

#include "omxilgenericportilif.h"
	
/**
class COmxILGenericPortILIF
Concrete implementation of the MILComponentPortIf that encapsulates the 
component's port specific behaviour according to OpenMAX specification.
*/
COmxILGenericPortILIF::COmxILGenericPortILIF(COmxILGenericILIF& aParent,
										     TPortDirection aPortDirection, 
											 TInt aIndex)
	:
	iPortDirection (aPortDirection),
	iPortIndex (aIndex),						
	iPortComponent(aParent)
	{	
	}

COmxILGenericPortILIF* COmxILGenericPortILIF::NewL(COmxILGenericILIF& aParent,
												   TPortDirection aPortDirection, 
												   TInt aIndex)
	{
	COmxILGenericPortILIF* self = new (ELeave) COmxILGenericPortILIF(aParent, aPortDirection, aIndex);
	return self;
	}
	
TInt COmxILGenericPortILIF::FillThisBuffer(CMMFBuffer& aBuffer)
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

TInt COmxILGenericPortILIF::EmptyThisBuffer(const CMMFBuffer& aBuffer)
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
	
TInt COmxILGenericPortILIF::TunnelRequest(MILComponentPortIf* aPort)
	{
	TInt error = KErrNotSupported;
	
	if (aPort)
		// This is the connet tunnel request
		{
		TPortDirection otherPortDir = aPort->PortDirection();
		COmxILGenericILIF* otherPortComponent = static_cast<COmxILGenericILIF*>(aPort->PortComponent());
		
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
				iTunneledPort = static_cast<COmxILGenericPortILIF*>(aPort);
				iTunneledPort->SetPortConnectedTo(this);
				}
			}
		}
	else
		// This is the disconnet tunnel request
		{
		TPortDirection otherPortDir = iTunneledPort->PortDirection();
		COmxILGenericILIF* otherPortComponent = static_cast<COmxILGenericILIF*> (iTunneledPort->PortComponent());

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

TInt COmxILGenericPortILIF::PortIndex() const
	{
	return iPortIndex;
	}

TPortDirection COmxILGenericPortILIF::PortDirection() const
	{
	return iPortDirection;
	}

CMMFBuffer* COmxILGenericPortILIF::CreateBufferL(TInt aBufferSize)
	{
	return iPortComponent.OmxAllocateBufferL(iPortIndex, aBufferSize);	
	}

TInt COmxILGenericPortILIF::UseBuffer(CMMFBuffer& aBuffer)
	{
	return iPortComponent.OmxUseBuffer(&aBuffer, iPortIndex);	
	}

TInt COmxILGenericPortILIF::FreeBuffer(CMMFBuffer* aBuffer)
	{
	return iPortComponent.OmxFreeBuffer(aBuffer);
	}

TInt COmxILGenericPortILIF::FlushPort()
	{
	return iPortComponent.OmxSendCommand(OMX_CommandFlush, iPortIndex, NULL);
	}

TInt COmxILGenericPortILIF::EnablePort()
	{
	return iPortComponent.OmxSendCommand(OMX_CommandPortEnable, iPortIndex, NULL);
	}
	
TInt COmxILGenericPortILIF::DisablePort()
	{
	return iPortComponent.OmxSendCommand(OMX_CommandPortDisable, iPortIndex, NULL);
	}

MILComponentIf* COmxILGenericPortILIF::PortComponent() const
	{
	return &iPortComponent;
	}

void COmxILGenericPortILIF::SetPortConnectedTo(COmxILGenericPortILIF* aPort)
	{
	iTunneledPort = aPort;	
	}


