// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "RecIclUtilBody.h"

/**
 * @internalComponent
 *
 * constructor for ICL Recognizer utility class
 */
CIclRecognizerUtil::CIclRecognizerUtil()
	{
	}

CIclRecognizerUtil::~CIclRecognizerUtil()
	{
	delete iBody;
	}

void CIclRecognizerUtil::ConstructL()
	{
	iBody = CIclRecognizerUtil::CBody::NewL();
	}

/**
 * @internalComponent
 *
 * Determine whether the supplied data header or file extension is recognized
 * and if so return the associated MIME type
 * @param	aImageData
 *          A descriptor containing the header. Set to KNullDesC8 for match by file extension.
 * @param	aFileName
 *			A file name for file extension matching. Set to KNullDesC for match by image data
 * @param   aMimeType
 *          A user-supplied descriptor in which the MIME type is returned
 * @return	ETrue if a match was found.
 *			EFalse if a match was not found.
 * @leave	This method may also leave with one of the system-wide error codes.
 * @post    If recognized, the caller's descriptor is filled with the MIME types
 */
EXPORT_C TBool CIclRecognizerUtil::GetMimeTypeL(const TDesC8& aImageData, const TDesC& aFileName, TDes8& aMimeType)
	{
	return iBody->GetMimeTypeL(aImageData, aFileName, aMimeType);
	}

/**
 * @internalComponent
 *
 * Static factory constructor. Uses two phase
 * construction and leaves nothing on the cleanup stack
 *
 * @leave KErrNoMemory
 * @return A pointer to the newly created CIclRecognizerUtil object
 *
 */
EXPORT_C CIclRecognizerUtil* CIclRecognizerUtil::NewL()
	{
	CIclRecognizerUtil* self=new (ELeave) CIclRecognizerUtil();   
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

