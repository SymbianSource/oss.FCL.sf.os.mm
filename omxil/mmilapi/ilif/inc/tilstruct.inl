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

#ifndef TILSTRUCT_INL
#define TILSTRUCT_INL

/**
Constructor
*/	
inline TILStruct::TILStruct()
	:
	iUid(KNullUid)
	{
	}
	
/**
Obtain the UID identifying the structure.
@return The identifying UID of the structure.
*/	
inline const TUid& TILStruct::Uid() const 
	{
	return iUid;
	}

/**
Constructor
@param aParameterStructure The structure to store
*/	
template <TInt32 U, class T>
inline TILStructParam<U,T>::TILStructParam(const T& aParameterStructure)
	{
	iUid = TUid::Uid(U);
	iBuf = TPckgBuf<T>(aParameterStructure);
	}

/**
Default Constructor
*/		
template <TInt32 U, class T>
inline TILStructParam<U,T>::TILStructParam()
	{
	iUid = TUid::Uid(U);
	}

/** 
Operator to return the structure represented by the class.
@return	The structure represented by the class.
*/
template <TInt32 U, class T>
inline TILStructParam<U,T>::operator T&()
	{
	return iBuf();		
	}

/** 
Static method to return the structure represented by TILStruct.
@param	A const reference to the base structure.
@return	A const pointer to the structure represented by the class.
*/
template <TInt32 U, class T>
inline const T* TILStructParam<U,T>::GetStructure(const TILStruct& aConfig)
	{
	const T* ptr  = &((static_cast<const TILStructParam<U,T>& >(aConfig)).iBuf());
	return ptr;
	}

/** 
Static method to return the structure represented by TILStruct.
@param	A reference to the base structure.
@return	A pointer to the structure represented by the class.
*/
template <TInt32 U, class T>
inline T* TILStructParam<U,T>::GetStructure(TILStruct& aConfig)
	{
	T* ptr  = &((static_cast<TILStructParam<U,T>& >(aConfig)).iBuf());
	return ptr;
	}



#endif  // TILSTRUCT_INL
