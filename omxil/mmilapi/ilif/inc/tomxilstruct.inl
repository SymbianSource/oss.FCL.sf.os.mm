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

#ifndef TOMXILSTRUCT_INL
#define TOMXILSTRUCT_INL


/**
Constructor

@param 	aParam 
		The pointer to the structure associated to the particular index stored in this wrapper class.
@param 	aIndex 
		An index to identify the definition of the structure pointed to by this wrapper class. 
*/	
inline TOMXILStructParam::TOMXILStructParam(TAny* aParam, TILStructIndex aIndex)
	: iParam(aParam), iIndex(aIndex)
{
	ASSERT(aParam);
	
	iUid = TUid::Uid(KUidOMXILStructType);
}


/** 
Method to return the pointer to the structure represented by the index stored in this TOMXILStructParam.
The pointer should be cast to the structure associated to the TILStructIndex stored TOMXILStructParam 

@return	A pointer to the OMX IL structure represented by this particular instance of the TOMXILStructParam
*/
inline TAny* TOMXILStructParam::Param() const
{
	// This is not a const method. The fields in Param could be modified by the client.
	return iParam;
}

	
/** 
Method to return the TILStructIndex.

@return	A TILStructIndex index
@see TILStructIndex
*/
inline TILStructIndex TOMXILStructParam::Index() const
{
	return iIndex;
}


#endif  // TOMXILSTRUCT_INL
