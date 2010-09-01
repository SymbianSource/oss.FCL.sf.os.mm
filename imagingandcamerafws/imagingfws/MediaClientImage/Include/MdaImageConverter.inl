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
//

#include "MdaImageConverterPriv.h"

inline CMdaImageDataReadUtilityPriv* CMdaImageDataReadUtility::Properties() const
	{
	return static_cast<CMdaImageDataReadUtilityPriv*>(iProperties);
	}

inline CMdaImageDataWriteUtilityPriv* CMdaImageDataWriteUtility::Properties() const
	{
	return static_cast<CMdaImageDataWriteUtilityPriv*>(iProperties);
	}

inline CMdaImageFileToBitmapUtilityPriv* CMdaImageFileToBitmapUtility::Properties() const
	{
	return static_cast<CMdaImageFileToBitmapUtilityPriv*>(iProperties);
	}

inline CMdaImageDescToBitmapUtilityPriv* CMdaImageDescToBitmapUtility::Properties() const
	{
	return static_cast<CMdaImageDescToBitmapUtilityPriv*>(iProperties);
	}

inline CMdaImageBitmapToFileUtilityPriv* CMdaImageBitmapToFileUtility::Properties() const
	{
	return static_cast<CMdaImageBitmapToFileUtilityPriv*>(iProperties);
	}

inline CMdaImageBitmapToDescUtilityPriv* CMdaImageBitmapToDescUtility::Properties() const
	{
	return static_cast<CMdaImageBitmapToDescUtilityPriv*>(iProperties);
	}

inline CMdaImageBitmapToBitmapUtilityPriv* CMdaImageBitmapToBitmapUtility::Properties() const
	{
	return static_cast<CMdaImageBitmapToBitmapUtilityPriv*>(iProperties);
	}
