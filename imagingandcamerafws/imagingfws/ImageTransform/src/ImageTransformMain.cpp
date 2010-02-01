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


#include "ImageTransformMain.h"
//#include "ImageTransformPlugin.h"
#include "ImageTransformFramework.h"

// Literal for panics.
_LIT(KIclPanicCategory, "ImageTransform");

// Global panic fn.
GLDEF_C void Panic(TImageTransformPanic aError)
	{
	User::Panic(KIclPanicCategory, aError);
	}


TImageParameterData::TImageParameterData()
	{
	Reset();
	}

void TImageParameterData::Reset()
	{
	iImageParameterDataFlag = ENull;
	iFilename = NULL;
	iData = NULL;
	iDataPtr = NULL;
	iMimeType = &KNullDesC8;
	iImageType = KNullUid;
	iImageSubType = KNullUid;
	}

void TImageParameterData::SetFilename(const TDesC& aFilename)
	{
	iImageParameterDataFlag = EFilename;
	iFilename = &aFilename;
	}

void TImageParameterData::SetData(const TDesC8& aData)
	{
	iImageParameterDataFlag = EData;
	iData = &aData;
	}

void TImageParameterData::SetDataPtr(HBufC8*& aDataPtr)
	{
	iImageParameterDataFlag = EDataPtr;
	iDataPtr = &aDataPtr;
	}

void TImageParameterData::SetMimeType(const TDesC8& aMimeType)
	{
	iMimeType = &aMimeType;
	}

void TImageParameterData::SetImageType(TUid aImageType, TUid aImageSubType)
	{
	iImageType = aImageType;
	iImageSubType = aImageSubType;
	}

/**
 *
 * Indicates whether the data source or destination desciptor contains a filename.
 *
 * @return	ETrue if the source or destination descriptor contains a filename, otherwise EFalse.
 * @panic   EUndefinedSourceType
 *          The source type has not been defined.
 */
TBool TImageParameterData::IsFilename() const
	{
	if (iImageParameterDataFlag == ENull)
		Panic(EUndefinedSourceType);
	return iImageParameterDataFlag == EFilename;
	}

/**
 *
 * Indicates whether the data source desciptor contains data.
 *
 * @return	ETrue if the source or destination descriptor contains data, otherwise EFalse.
 * @panic   EUndefinedSourceType
 *          The source type has not been defined.
 */
TBool TImageParameterData::IsData() const
	{
	if (iImageParameterDataFlag == ENull)
		{
		Panic(EUndefinedSourceType);
		}
	return ((iImageParameterDataFlag == EData) || (iImageParameterDataFlag == EDataPtr));
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
	if (iImageParameterDataFlag != EFilename)
		Panic(EUndefinedSourceType);
	return *iFilename;
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
	if (iImageParameterDataFlag != EData)
		Panic(EUndefinedSourceType);
	return *iData;
	}

HBufC8*& TImageParameterData::DataPtr() const
	{
	if (iImageParameterDataFlag != EDataPtr)
		Panic(EUndefinedSourceType);
	return *iDataPtr;
	}

const TDesC8& TImageParameterData::MimeType() const
	{
	return *iMimeType;
	}

const TUid TImageParameterData::ImageType() const
	{
	return iImageType;
	}

const TUid TImageParameterData::ImageSubType() const
	{
	return iImageSubType;
	}

TBool TImageParameterData::IsDataTypeDefined() const
	{
	return (iImageParameterDataFlag == ENull)?EFalse:ETrue;
	}
