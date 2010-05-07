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

#ifndef ILCOMPONENTIF_INL
#define ILCOMPONENTIF_INL

#include <ecom/ecom.h>

#include "ilcomponentif.h"

/**
	Create a CILComponentIf object with a known implementation Uid.
	Asks ECOM plugin framework to instantiate this concret plugin implementation.
	Will leave if it is not found (KErrNotFound).
	@param aUid   -  The Uid of a plugin implementation
	@return An instantiated CILComponentIf derived object from ECom.
 */
inline CILComponentIf* CILComponentIf::CreateImplementationL(TUid aImplementationUid)
	{
	TAny* ptr = REComSession::CreateImplementationL(aImplementationUid , _FOFF(CILComponentIf, iDtor_ID_Key));
	return static_cast<CILComponentIf*>(ptr);
	}

/**
	Interface base class destructor
*/	
inline CILComponentIf::~CILComponentIf()
	{
	// Destroy any instance variables and then inform ecom that this specific
	// instance of the interface has been destroyed.
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}

#endif // ILCOMPONENTIF_INL
