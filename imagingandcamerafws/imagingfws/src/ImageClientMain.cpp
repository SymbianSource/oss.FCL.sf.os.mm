// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "icl/ImageCodec.h"
#include "ImageClientMain.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/imagecodecdef.h>
#endif

_LIT(KIclPanicCategory, "ImageConversion");

// Global panic function
GLDEF_C void Panic(TIclPanic aError)
	{
	User::Panic(KIclPanicCategory, aError);
	}

TImageParameterData::TImageParameterData()
	: iImageParameterDataFlag(ENull)
	{
	}

void TImageParameterData::SetFilenamePtr(const TDesC* aSourceFilenamePtr)
	{
	iImageParameterDataFlag = EFilename;
	iSourceFilenamePtr = aSourceFilenamePtr;
	}

void TImageParameterData::SetDataPtr(const TDesC8* aSourceDataPtr)
	{
	iImageParameterDataFlag = EData;
	iSourceDataPtr = aSourceDataPtr;
	}

/**
Indicates whether the data source desciptor contains data or a filename.

@return	A boolean indicating if the source descriptor contains a filename. ETrue if it does contain
        a filename, otherwise EFalse.

@panic  EUndefinedSourceType
        The source type has not been defined.
*/
EXPORT_C TBool TImageParameterData::IsFilename() const
	{
	if (iImageParameterDataFlag == ENull)
		Panic(EUndefinedSourceType);
	return iImageParameterDataFlag == EFilename;
	}

/**
Returns a pointer to the filename from the source descriptor.

@return A pointer to the filename source descriptor.

@panic  EUndefinedSourceType
        The source has not been defined as a file source.
*/
EXPORT_C const TDesC* TImageParameterData::SourceFilenamePtr() const
	{
	if (iImageParameterDataFlag != EFilename)
		Panic(EUndefinedSourceType);
	return iSourceFilenamePtr;
	}

/**
Returns a pointer to the data from the source descriptor.

@return	A pointer to the data source descriptor.

@panic  EUndefinedSourceType
        The source has not been defined as a data source.
*/
EXPORT_C const TDesC8* TImageParameterData::SourceDataPtr() const
	{
	if (iImageParameterDataFlag != EData)
		Panic(EUndefinedSourceType);
	return iSourceDataPtr;
	}

TBool TImageParameterData::IsDataTypeDefined() const
	{
	return (iImageParameterDataFlag == ENull)?EFalse:ETrue;
	}
