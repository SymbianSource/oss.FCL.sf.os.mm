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

#ifndef ILCOMPONENTIF_H
#define ILCOMPONENTIF_H

#include <e32base.h>
#include <ecom/ecom.h>

#include "milcomponentif.h"

/**
	Generic ECom plugin class for creating MILComponentIf instances.
	@see MILComponentIf
*/
class CILComponentIf :	public CBase,
						public MILComponentIf
	{
public:
	inline static CILComponentIf* CreateImplementationL(TUid aImplementationUid);

	inline virtual ~CILComponentIf();

private:
	TUid iDtor_ID_Key;	
	};

#include "ilcomponentif.inl"

#endif // ILCOMPONENTIF_H
