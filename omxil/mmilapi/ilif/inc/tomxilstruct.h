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
 @internalTechnology
 @prototype
*/

#ifndef TOMXILSTRUCT_H
#define TOMXILSTRUCT_H

#include "tilstruct.h"
#include <openmax/il/khronos/v1_x/OMX_Component.h>

/** 
A typedef to symbianize the OpenMAX IL index type structure.
*/	
typedef OMX_INDEXTYPE TILStructIndex;


/**
Derived class to create a TILStruct configuration given an index and a pointer to a parameter structure.
The index identifies the param structure
*/
class TOMXILStructParam : public TILStruct
	{
public:
	inline TOMXILStructParam(TAny* aParam, TILStructIndex aIndex);
	inline TAny* Param() const;
	inline TILStructIndex Index() const;

private:
	TAny* iParam;  // Not owned
	TILStructIndex iIndex;
	};

#include "tomxilstruct.inl"

#endif  // TOMXILSTRUCT_H
