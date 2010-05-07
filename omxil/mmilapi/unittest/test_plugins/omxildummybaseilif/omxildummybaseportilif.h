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

#ifndef OMXILDUMMYBASEPORTILIF_H
#define OMXILDUMMYBASEPORTILIF_H

#include "milcomponentportif.h"
#include "omxildummybaseilif.h"

	
/**
Concrete implementation of the MILComponentPortIf that encapsulates the 
component's port specific behaviour according to OpenMAX specification.
*/
class COmxILDummyBasePortILIF : public CBase,
                                public MILComponentPortIf
	{
public:
	static COmxILDummyBasePortILIF* NewL(COmxILDummyBaseILIF& aParent, TPortDirection aPortDirection, TInt aIndex);

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
	
	void SetPortConnectedTo(COmxILDummyBasePortILIF* aPort);
private:
	COmxILDummyBasePortILIF(COmxILDummyBaseILIF& aParent, TPortDirection aPortDirection, TInt aIndex);
	
private:
	TPortDirection iPortDirection;
	TInt iPortIndex;	

	COmxILDummyBaseILIF& iPortComponent;
	COmxILDummyBasePortILIF* iTunneledPort;
	};

#endif // OMXILDUMMYBASEPORTILIF_H
