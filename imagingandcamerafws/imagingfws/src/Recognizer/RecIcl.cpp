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

#include <f32file.h>
#include <barsread.h>

#include "ImageDisplayRecognizer.h"
#include "RecIcl.h"
#include "ImageResolverAPI.h"	// for KImageHeaderSize

const TInt KMimeIclRecognizerValue = 0x101F7C0B;
const TUid KUidMimeIclRecognizer = {KMimeIclRecognizerValue};
const TInt KIclRecognizerPriority = 10; // The recognizer priority is set to 10(a value between ENormal and EHigh)

// CApaIclRecognizer

/**
 * @internalAll
 *
 * call base constructor with the recognizer's UID and confidence level
 * The recognizer priority is set to 10(a value between ENormal and EHigh) to allow 
 * third-party recognizers to specify high priority.
 */
CApaIclRecognizer::CApaIclRecognizer()
	:CApaDataRecognizerType(KUidMimeIclRecognizer,KIclRecognizerPriority)
	{
	}

CApaIclRecognizer::~CApaIclRecognizer()
	{
	delete iIclRecognizer;
	}

/**
 * @internalAll
 *
 * return the supposed minimum buffer size we need to 
 * successfully recognize the data
 */
TUint CApaIclRecognizer::PreferredBufSize()
	{
	return KImageHeaderSize;	
	}

/**
 * @internalAll
 *
 * Gets one of the data (MIME) types that the recognizer can recognize.
 */
TDataType CApaIclRecognizer::SupportedDataTypeL(TInt aIndex) const
	{
	TDataType temp(iIclRecognizer->SupportedDataTypeL(aIndex));
	return temp;
	}


/**
 * @internalAll
 *
 * attempt to recognize the data
 * this recognizer only attempts to match the data - not the file name
 *
 * NB if the file is not recognized, this function should NOT leave :
 * it should instead set iConfidence = ENotRecognized and return
 * the function should only leave if there is an out-of-memory condition
 */
void CApaIclRecognizer::DoRecognizeL(const TDesC& aName , const TDesC8& aBuffer)
	{
	CIclRecognizer::TMatchMethod matchMethod = iIclRecognizer->MatchL(aBuffer, aName);
	
	// return whether the data was matched by setting iConfidence
	// if matched the MIME type is returned in iDataType
	if (matchMethod == CIclRecognizer::ENotMatched)
		{	
		iConfidence = CApaDataRecognizerType::ENotRecognized;
		}
	else
		{
		iDataType = iIclRecognizer->MimeString();

		if (matchMethod == CIclRecognizer::EBySignature)
			iConfidence = CApaDataRecognizerType::EProbable;
		else
			{
			ASSERT(matchMethod==CIclRecognizer::EByName);
			iConfidence = CApaDataRecognizerType::EPossible;
			}
		}
	}

void CApaIclRecognizer::ConstructL()
	{
	iIclRecognizer = CIclRecognizer::NewL();
	iCountDataTypes = iIclRecognizer->NumMimeTypes();
	}

CApaIclRecognizer* CApaIclRecognizer::NewL()
	{
	CApaIclRecognizer* self = new (ELeave) CApaIclRecognizer();   
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// CIclRecognizer - the main utility class owner by CApaIclRecognizer

CIclRecognizer::CIclRecognizer()
	{
	}

CIclRecognizer::~CIclRecognizer()
	{
	delete iIclRecognizerUtil;
	delete iImgDisplayRecognizer;
	iFileExtensionMIMETypeArray.ResetAndDestroy();
	}

void CIclRecognizer::ConstructL()
	{
	iImgDisplayRecognizer= CImgDisplayMimeTypeRecognizer::NewL();
	iIclRecognizerUtil = CIclRecognizerUtil::NewL();
	BuildListL();
	}

CIclRecognizer* CIclRecognizer::NewL()
	{
	CIclRecognizer* self = new (ELeave) CIclRecognizer();   
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
 * @internalComponent
 *
 * return the number of MIME types supported 
 */
TInt CIclRecognizer::NumMimeTypes() const
	{
	return(iFileExtensionMIMETypeArray.Count());
	}

/**
 * @internalComponent
 *
 * call into the ImageConversion DLL to get
 * a list of supported MIME types
 */
void CIclRecognizer::BuildListL()
	{
	iFileExtensionMIMETypeArray.ResetAndDestroy();
	CImageDecoder::GetFileTypesL(iFileExtensionMIMETypeArray);
	iImgDisplayRecognizer->GetFileTypesL(iFileExtensionMIMETypeArray);
	}

/**
 * @internalComponent
 *
 * Get one of the data (MIME) types that ICL can recognize.
 */
const TDesC8& CIclRecognizer::SupportedDataTypeL(TInt aIndex) const
	{
	if ((aIndex < 0) || (aIndex >= iFileExtensionMIMETypeArray.Count()))
		{
		User::Leave(KErrArgument);
		}
	
	return(iFileExtensionMIMETypeArray[aIndex]->MIMEType());
	}

/**
 * @internalComponent
 *
 * get a reference to the last MIME type string successfully matched
 */
const TDesC8& CIclRecognizer::MimeString() const
	{
	return iMimeString;
	}

/**
 * @internalComponent
 *
 * attempt to recognize the data
 */
CIclRecognizer::TMatchMethod CIclRecognizer::MatchL(const TDesC8& aBuffer, const TDesC& aFileName)
	{
	TMatchMethod bestMatchMethod = ENotMatched;

	//Try to recognize image data
	TBool matchFound = EFalse;
	matchFound = iIclRecognizerUtil->GetMimeTypeL(aBuffer, KNullDesC, iMimeString);
	if (!matchFound)
		{
		matchFound = iImgDisplayRecognizer->GetMimeTypeL(aBuffer, KNullDesC, iMimeString);
		}

	if(matchFound)
		{
		bestMatchMethod = EBySignature;
		}

	if (aFileName != KNullDesC && !matchFound)
		{//Try to recognize file extension
		matchFound = iIclRecognizerUtil->GetMimeTypeL(KNullDesC8, aFileName, iMimeString);
		if (!matchFound)
			{
			matchFound = iImgDisplayRecognizer->GetMimeTypeL(KNullDesC8, aFileName, iMimeString);
			}		
		if(matchFound)
			{
			bestMatchMethod = EByName;
			}
		}

	return bestMatchMethod;
	}


#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>

const TImplementationProxy ImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY(0x101F7C40, CApaIclRecognizer::NewL)
	};


EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }



