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

#ifndef OMXILGENERICPORTILIF_H
#define OMXILGENERICPORTILIF_H

#include "milcomponentportif.h"
#include "omxilgenericilif.h"

	
/**
Concrete implementation of the MILComponentPortIf that encapsulates the 
component's port specific behaviour according to OpenMAX specification.
*/
class COmxILGenericPortILIF  :  public CBase,
                                public MILComponentPortIf
	{
public:
	static COmxILGenericPortILIF* NewL(COmxILGenericILIF& aParent, TPortDirection aPortDirection, TInt aIndex);

	// MILComponentPortIf
	TInt FillThisBuffer(CMMFBuffer& aBuffer);
	TInt EmptyThisBuffer(const CMMFBuffer& aBuffer);
	TInt TunnelRequest(MILComponentPortIf* aPort);
	TInt PortIndex() const;
	TPortDirection PortDirection() const;
	CMMFBuffer* CreateBufferL(TInt aBufferSize);
	TInt UseBuffer(CMMFBuffer& aBuffer);
	TInt FreeBuffer(CMMFBuffer* aBuffer);
	TInt FlushPort();
	TInt EnablePort();
	TInt DisablePort();
	MILComponentIf* PortComponent() const; 
	
	void SetPortConnectedTo(COmxILGenericPortILIF* aPort);
	
private:
	COmxILGenericPortILIF(COmxILGenericILIF& aParent, TPortDirection aPortDirection, TInt aIndex);
	
private:
	TPortDirection iPortDirection;
	TInt iPortIndex;	
	COmxILGenericILIF& iPortComponent;
	COmxILGenericPortILIF* iTunneledPort;
	};

#endif // OMXILGENERICPORTILIF_H
