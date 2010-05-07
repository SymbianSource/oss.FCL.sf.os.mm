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

#ifndef TILSTRUCT_H
#define TILSTRUCT_H

#include <e32base.h>

/**
Configuration structure base class. 
Represents a configuration structure identified by a base class.
*/
class TILStruct
	{
public:
	inline const TUid& Uid() const;

protected:
	inline TILStruct();

protected:
	TUid iUid;
	};

/**
Template class to create a TILStruct configuration given a structure and an unique UID to identify it.
*/
template<TInt32 U , class T>
class TILStructParam : public TILStruct
	{
public:
	inline TILStructParam(const T& aParameterStructure);
	inline TILStructParam();
	inline operator T&();
	inline static const T* GetStructure(const TILStruct& aConfig);	
	inline static T* GetStructure(TILStruct& aConfig);	
private:
	TPckgBuf<T> iBuf;
	};

#include "tilstruct.inl"

#endif  // TILSTRUCT_H
