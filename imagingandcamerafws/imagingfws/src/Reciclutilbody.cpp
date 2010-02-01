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
// This is the body of the ICL recognizer utility class
// 
//

#include "ImageConversion.h"
#include "ImageResolverAPI.h"
#include "ImageConvResolvrUtils.h"
#include "ImageRelay.h"
#include "EnDecoderUtils.h"
#include "RecIclUtilBody.h"

// ICL Recognizer Utility class body constructor
CIclRecognizerUtil::CBody::CBody():
	CActive(CActive::EPriorityStandard)
	{
	}

CIclRecognizerUtil::CBody::~CBody()
	{
	Cancel();
	iEcomSession.Close();
	iFs.Close();
	iPluginArray.ResetAndDestroy();
	}

// Request ECom to be notified when interface implementation registration 
// data changes so that we can refresh the list of interface implementations.
void CIclRecognizerUtil::CBody::StartNotification()
	{
	iEcomSession.NotifyOnChange(iStatus);
	SetActive();
	}

// build a list of interface implementation objects
void CIclRecognizerUtil::CBody::BuildControllerListL()
	{
	iPluginArray.ResetAndDestroy();
	REComSession::ListImplementationsL(
		KImageDecoderInterfaceUid, 
		iPluginArray);
	}

void CIclRecognizerUtil::CBody::ConstructL()
	{
	BuildControllerListL();

	CActiveScheduler::Add(this);

	User::LeaveIfError(iFs.Connect());

    iEcomSession = REComSession::OpenL();

	// request notification from ECOM of any file system changes
	StartNotification();
	}

void CIclRecognizerUtil::CBody::RunL()
	{
	BuildControllerListL();
	StartNotification();
	}

void CIclRecognizerUtil::CBody::DoCancel()
	{
	if (iStatus == KRequestPending)
		iEcomSession.CancelNotifyOnChange(iStatus);
	}

// Determine whether the supplied data header or file extension is recognized
// and if so return the associated MIME type
// @param	aImageData
//          A descriptor containing the header. Set to KNullDesC8 for match by file extension.
// @param	aFileName
//			A file name for file extension matching. Set to KNullDesC for match by image data
// @param   aMimeType
//          A user-supplied descriptor in which the MIME type is returned
// @return	ETrue if a match was found.
//			EFalse if a match was not found.
// @leave	This method may also leave with one of the system-wide error codes.
// @post    If recognized, the caller's descriptor is filled with the MIME types
TBool CIclRecognizerUtil::CBody::GetMimeTypeL(const TDesC8& aImageData, const TDesC& aFileName, TDes8& aMimeType)
	{
	TBool matchImageData = ETrue;
	HBufC* fileSuffix = NULL;

	if(aFileName==KNullDesC)
		{// match on image data
		if (aImageData.Length() < KImageHeaderSize) // There is not enough data in this source
			return EFalse; 
		}
	else
		{// match on file extension
		// Ensure aFileName is < KMaxFileName
		if ( aFileName.Length() > KMaxFileName )
			{
			User::Leave(KErrBadName);
			}
		
		TParse fileName;
 
 		TDes* const fName = new(ELeave) TFileName;
 		
 		CleanupStack::PushL(fName);
 		
 		fName->Copy(aFileName);
 
 		// if illegalifier character has been added to the file name,
 		// remove the illegalifier and pass the file name only to the parser.
 		if (fName->Match(_L("::*")) == 0)
 			{
 			fName->Delete(0, 2);
 			}
 
 		fileName.Set(*fName, NULL, NULL);
 
 		CleanupStack::PopAndDestroy(fName);


		//No file extension
		if (!fileName.ExtPresent())	
			return EFalse;
		
		//Get the suffix
		fileSuffix = fileName.Ext().AllocLC();

		matchImageData = EFalse;
		}

	TBool matchFound = EFalse;

	// loop through every plugin
	// until we find one that matches the file extension
	const TInt count = iPluginArray.Count();
	TInt oldMatchLevel = 0; // number of characters matched in best match

	for(TInt index = 0; index < count && !matchFound; index++)
		{
		const CImplementationInformation& impData = *(iPluginArray[index]);

		COpaqueDataParse *parse = NULL;
		TRAPD(error, parse = COpaqueDataParse::NewL(impData.OpaqueData()));
		if (error != KErrNone)
			{
			if (error==KErrNotSupported)
				continue;
			else
				User::Leave(error);
			}
		CleanupStack::PushL(parse);

		// we can assume the version is valid as it is checked in 
		// COpaqueDataParse::ConstructL() which leaves if not valid
		ASSERT(parse->Version() <= KIclPluginFrameworkVersionMax);
		
		if (!impData.Disabled() && !parse->OnlyUidsAvail())
			{
			if(matchImageData)
				{// match image data 
				TInt matchLevel = 0;
				if (CImageConversionResolverUtils::Match(aImageData, impData.DataType(), matchLevel))
					{
					TInt error = KErrNone;
					// do we need to create a decoder to fully verify the header data ?
					if (parse->IsOpenNeededToRecognize())
						{
						CImageDecoder* decoder = NULL;
           				TRAP(error, decoder = CImageDecoder::DataNewL(iFs, aImageData));
						delete decoder;
						if (error==KErrNoMemory) // handle OOM, but assume everything else is a do not recognize
							User::Leave(KErrNoMemory);
						else if (error==KErrUnderflow && aImageData.Length()>=KImageHeaderSize) 
							{
							// assuming we have enough data, means the format requires more
							// might as well assume OK
							error=KErrNone;
							}
						}
					if(error == KErrNone && oldMatchLevel < matchLevel)
						{
						// either found a match or a better match
						oldMatchLevel = matchLevel;
						parse->EnsureMIMETypesReadL();
						aMimeType = (parse->MIMETypesCount()==0) ? KNullDesC8() : parse->MIMEType(0);
						}
					}
				}
			else
				{// match file extension
				ASSERT(!matchImageData);
				ASSERT(fileSuffix!=NULL);

				if(parse->IsRecognizeAgainstSuffix())
					{ //If codec has file extensions and allow matching on them
					parse->EnsureExtnsReadL();
					const TInt numExtns = parse->ExtnsCount();
					for (TInt index2 = 0; index2 < numExtns; index2++)
						{
						const TDesC8& extn = parse->Extn(index2);
						if (COpaqueDataParse::CompareFileSuffixL(*fileSuffix, extn))
							{
							// return the first MIME type listed for this plugin
							matchFound = ETrue;
							parse->EnsureMIMETypesReadL();
							aMimeType = (parse->MIMETypesCount()==0) ? KNullDesC8() : parse->MIMEType(0);
							break;
							}
						}
					}
				}
			}
		CleanupStack::PopAndDestroy(parse);
		}
	
	if(oldMatchLevel>0) 
			{
			// looked at all plugins now 
			// return the best MIME type listed for this plugin
			matchFound = ETrue;
			}

	if(!matchImageData)
		CleanupStack::PopAndDestroy(fileSuffix);

	return matchFound;
	}


// Static factory constructor. Uses two phase
// construction and leaves nothing on the cleanup stack
// 
// @leave KErrNoMemory
// @return A pointer to the newly created CIclRecognizerUtil::CBody object
// 
CIclRecognizerUtil::CBody* CIclRecognizerUtil::CBody::NewL()
	{
	CBody* self=new (ELeave) CBody();   
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

