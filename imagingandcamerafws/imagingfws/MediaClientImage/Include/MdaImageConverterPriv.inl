// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// MdaImageConverter.inl
// Inline implementation of the API interfaces defined in MdaImageConverter.h
// 
//

#ifndef __MDAIMAGECONVERTERPRIV_INL__
#define __MDAIMAGECONVERTERPRIV_INL__

#include <mdaimageconverter.h>

inline CMdaImageDataReadUtility* CMdaImageDataReadUtilityPriv::Parent() const
	{
	return static_cast<CMdaImageDataReadUtility*>(iParent);
	}

inline CMdaImageDataWriteUtility* CMdaImageDataWriteUtilityPriv::Parent() const
	{
	return static_cast<CMdaImageDataWriteUtility*>(iParent);
	}

inline CMdaImageFileToBitmapUtility* CMdaImageFileToBitmapUtilityPriv::Parent() const
	{
	return static_cast<CMdaImageFileToBitmapUtility*>(iParent);
	}

inline CMdaImageDescToBitmapUtility* CMdaImageDescToBitmapUtilityPriv::Parent() const
	{
	return static_cast<CMdaImageDescToBitmapUtility*>(iParent);
	}

inline CMdaImageBitmapToFileUtility* CMdaImageBitmapToFileUtilityPriv::Parent() const
	{
	return static_cast<CMdaImageBitmapToFileUtility*>(iParent);
	}

inline CMdaImageBitmapToDescUtility* CMdaImageBitmapToDescUtilityPriv::Parent() const
	{
	return static_cast<CMdaImageBitmapToDescUtility*>(iParent);
	}

inline CMdaImageBitmapToBitmapUtility* CMdaImageBitmapToBitmapUtilityPriv::Parent() const
	{
	return static_cast<CMdaImageBitmapToBitmapUtility*>(iParent);
	}


#endif // __MDAIMAGECONVERTERPRIV_INL__
