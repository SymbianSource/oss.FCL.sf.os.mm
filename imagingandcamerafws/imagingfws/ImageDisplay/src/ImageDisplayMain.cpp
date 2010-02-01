// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @internalComponent 
*/

#include "ImageDisplayMain.h"
#include "ImageDisplayFramework.h"

// Literal for panics.
_LIT(KIclPanicCategory, "ImageDisplay");

// Global panic fn.
void Panic(TImageDisplayPanic aError)
	{
	User::Panic(KIclPanicCategory, aError);
	}


void TImageParameterData::Reset()
	{
	iImageParameterDataFlag = CImageDisplayPlugin::EImgSrcNotDefined;
	iFilename.Set(KNullDesC);
	iData.Set(KNullDesC8);
	iMimeType.Set(KNullDesC8);
	iImageType = KNullUid;
	iImageSubType = KNullUid;
	}

void TImageParameterData::SetFilename(const TDesC& aFilename)
	{
	iImageParameterDataFlag = CImageDisplayPlugin::EImgSrcFileName;
	iFilename.Set(aFilename);
	}

void TImageParameterData::SetFileHandle(RFile& aFileHandle)
	{
	iImageParameterDataFlag = CImageDisplayPlugin::EImgSrcFileHandle;
	iFileHandle = &aFileHandle;
	}

void TImageParameterData::SetData(const TDesC8& aData)
	{
	iImageParameterDataFlag = CImageDisplayPlugin::EImgSrcDescriptor;
	iData.Set(aData);
	}

void TImageParameterData::SetImageType(TUid aImageType, TUid aImageSubType)
	{
	iImageType = aImageType;
	iImageSubType = aImageSubType;
	}

/**
 *
 * Retrieves a pointer to the filename from the source descriptor.
 *
 * @return	A pointer to the filename source descriptor.
 * @panic   EUndefinedSourceType
 *          The source has not been defined as a file source.
 */
const TDesC& TImageParameterData::Filename() const
	{
	if (iImageParameterDataFlag != CImageDisplayPlugin::EImgSrcFileName)
		{
		Panic(EUndefinedSourceType);
		}
		
	return iFilename;
	}

RFile& TImageParameterData::FileHandle() const
	{
	if (iImageParameterDataFlag != CImageDisplayPlugin::EImgSrcFileHandle)
		{
		Panic(EUndefinedSourceType);
		}
	return *iFileHandle;
	}

/**
 *
 * Retrieves a pointer to the data from the source descriptor.
 *
 * @return	A pointer to the data source descriptor.
 * @panic   EUndefinedSourceType
 *          The source has not been defined as a data source.
 */
const TDesC8& TImageParameterData::Data() const
	{
	if (iImageParameterDataFlag != CImageDisplayPlugin::EImgSrcDescriptor)
		{
		Panic(EUndefinedSourceType);
		}
	return iData;
	}

