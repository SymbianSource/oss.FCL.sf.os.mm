/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/


/**
   @file
   @internalComponent
*/

namespace
{
	// local definition of Min
	// this is to avoid ambiguous overload issues with the templated Min definitions in e32cmn.h
	inline TUint Min(TUint left, TUint right)
		{
		return left < right ? left : right;
		}

}
