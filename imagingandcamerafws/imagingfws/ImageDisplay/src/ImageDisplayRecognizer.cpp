// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**    @file
   @internalComponent */
#include "ImageDisplayFramework.h"

#include "ImageDisplay.hrh"
#include "../../src/ImageUtils.h"
#include "ImageDisplayResolvrUtils.h"

#include "ImageDisplayRecognizer.h"

NONSHARABLE_CLASS( CImgDisplayMimeTypeRecognizer::CBody) : public CActive
   	{
public:
   	static CBody* NewL();
   	~CBody();
   	TBool GetTypeByFileDataL(const TDesC8& aImageData, TDes8& aMimeType);
   	TBool GetTypeByFileNameL(const TDesC& aFileName, TDes8& aMimeType);
   	void GetFileTypesL(RFileExtensionMIMETypeArray& aExtArray);

protected:

	//from CActive
	virtual void DoCancel();
	virtual void RunL();
private:
	CBody();
	void ConstructL();
	void StartNotification();
	void BuildPluginListL();

private:
   	/** an array of pointers to CImplementationInformation objects */
   	RImplInfoPtrArray iPluginArray;
   	/** for opening a session to the ECom server */
   	REComSession iEcomSession;
    /** used when creating decoders */
    RFs iFs;
	};

/**
 	object factory function
 	@return class instance pointer
*/
EXPORT_C CImgDisplayMimeTypeRecognizer* CImgDisplayMimeTypeRecognizer::NewL()
	{
	CImgDisplayMimeTypeRecognizer* self=new (ELeave) CImgDisplayMimeTypeRecognizer();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CImgDisplayMimeTypeRecognizer::CImgDisplayMimeTypeRecognizer()
	{
	}
	
/**
	destructor
*/
CImgDisplayMimeTypeRecognizer::~CImgDisplayMimeTypeRecognizer()
	{
	delete iBody;
	}
	
void CImgDisplayMimeTypeRecognizer::ConstructL()
	{
	iBody = CBody::NewL();
	}
	
/**
	@param aExtArray array that will be filled with supported file extension information. Information is appended to existing array.
*/
EXPORT_C void CImgDisplayMimeTypeRecognizer::GetFileTypesL(RFileExtensionMIMETypeArray& aExtArray)
	{
	iBody->GetFileTypesL(aExtArray);
	}
	
/**
	determines Mime type by using filename and its content
	@param aImageData file data
	@param aFileName  file name
	@param aMimeType reference to a buffer that would be given mime type value upon successfull recognition
	@return ETrue if Mime type has been recognized
*/
EXPORT_C TBool CImgDisplayMimeTypeRecognizer::GetMimeTypeL(const TDesC8& aImageData, const TDesC& aFileName, TDes8& aMimeType)
	{
	TBool gotType=EFalse;
	aMimeType.SetLength(0);
	if (aImageData.Length()!=0)
		{
		gotType=iBody->GetTypeByFileDataL(aImageData, aMimeType);
		}
	if (!gotType && aFileName.Length()!=0)
		{
		gotType=iBody->GetTypeByFileNameL(aFileName, aMimeType);
		}
	return gotType;
	}
	
CImgDisplayMimeTypeRecognizer::CBody* CImgDisplayMimeTypeRecognizer::CBody::NewL()
	{
	CImgDisplayMimeTypeRecognizer::CBody* self=new (ELeave) CImgDisplayMimeTypeRecognizer::CBody();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CImgDisplayMimeTypeRecognizer::CBody::CBody():
	CActive(CActive::EPriorityStandard)
	{
	}

CImgDisplayMimeTypeRecognizer::CBody::~CBody()
	{
	Cancel();
	iEcomSession.Close();
	iFs.Close();
	iPluginArray.ResetAndDestroy();
	}

// Request ECom to be notified when interface implementation registration 
// data changes so that we can refresh the list of interface implementations.
void CImgDisplayMimeTypeRecognizer::CBody::StartNotification()
	{
	iEcomSession.NotifyOnChange(iStatus);
	SetActive();
	}

// build a list of interface implementation objects
void CImgDisplayMimeTypeRecognizer::CBody::BuildPluginListL()
	{
	iPluginArray.ResetAndDestroy();
	REComSession::ListImplementationsL(
		TUid::Uid(KUidDisplayInterface), 
		iPluginArray);
	}

inline
TBool IsExtAlreadyAdded(const RFileExtensionMIMETypeArray& aExtArray, const TDesC8& aExt)
	{
	TFileName extU;
	extU.Copy(aExt);
	for (TInt i=aExtArray.Count(); i--;)
		{
		const TDesC& ext=(*aExtArray[i]).FileExtension();
		if (ext.Length()==extU.Length() && extU.CompareF(ext)==0)
			{
			return ETrue;
			}
		}
	return EFalse;
	}
	
inline
TBool IsMimeAlreadyAdded(const RFileExtensionMIMETypeArray& aExtArray, const TDesC8& aMime)
	{
	for (TInt i=aExtArray.Count(); i--;)
		{
		const TDesC8& mime=(*aExtArray[i]).MIMEType();
		if (mime.Length()==aMime.Length() &&  aMime.CompareF(mime)==0)
			{
			return ETrue;
			}
		}
	return EFalse;
	}

void CImgDisplayMimeTypeRecognizer::CBody::GetFileTypesL(RFileExtensionMIMETypeArray& aExtArray)
	{
	for (TInt i=iPluginArray.Count(); i--;)
		{
		const CImplementationInformation& impData = *(iPluginArray[i]);
		COpaqueDataParse* parse = COpaqueDataParse::NewLC(impData.OpaqueData());

		parse->EnsureMIMETypesReadL();
		
		if (parse->MIMEType(0).Length() && !IsMimeAlreadyAdded(aExtArray, parse->MIMEType(0)))
			{
			parse->EnsureExtnsReadL();
			for (TInt extN=parse->ExtnsCount(); extN--;)
				{
				if (parse->Extn(extN).Length()==0 || IsExtAlreadyAdded(aExtArray, parse->Extn(extN)))
					{
					continue;
					}
				CFileExtensionMIMEType* ext=CFileExtensionMIMEType::NewLC(
												parse->Extn(extN),
												parse->MIMEType(0), // we always use the FIRST MIME type
												impData.DisplayName(),
												parse->ImageTypeUid(),
												parse->ImageSubTypeUid(),
												impData.ImplementationUid()
											);
				User::LeaveIfError( aExtArray.Append(ext) );
				CleanupStack::Pop(ext);
				}
			}
		CleanupStack::PopAndDestroy(parse);
		}
	}

void CImgDisplayMimeTypeRecognizer::CBody::ConstructL()
	{
	BuildPluginListL();
	CActiveScheduler::Add(this);
	User::LeaveIfError(iFs.Connect());
    iEcomSession = REComSession::OpenL();
	// request notification from ECOM of any file system changes
	StartNotification();
	}

void CImgDisplayMimeTypeRecognizer::CBody::RunL()
	{
	BuildPluginListL();
	StartNotification();
	}

void CImgDisplayMimeTypeRecognizer::CBody::DoCancel()
	{
	if (iStatus == KRequestPending)
		{
		iEcomSession.CancelNotifyOnChange(iStatus);
		}
	}

TBool CImgDisplayMimeTypeRecognizer::CBody::GetTypeByFileNameL(
	const TDesC& aFileName,
	TDes8& aMimeType)
	{
	aMimeType.SetLength(0);
	TParse fileName;
	fileName.Set(aFileName,NULL,NULL);

	//No file extension
	if (!fileName.ExtPresent())	
		{
		return EFalse;
		}
	const TPtrC suffix(fileName.Ext());
		
	for (TInt i=iPluginArray.Count(); i--;)
		{
		const CImplementationInformation& impData = *(iPluginArray[i]);
		COpaqueDataParse* parse = COpaqueDataParse::NewLC(impData.OpaqueData());
		parse->EnsureExtnsReadL();
		TFileName ext;
		TBool match=EFalse;
		for (TInt extN=0; !match && extN<parse->ExtnsCount(); ++extN)
			{
			ext.Copy(parse->Extn(extN));
			match=(ext.Length()==suffix.Length() && ext.CompareF(suffix)==0);
			}
		if (match)
			{			
			parse->EnsureMIMETypesReadL();
			aMimeType.Copy(parse->MIMEType(0)); // we always use the FIRST MIME type
			}
		CleanupStack::PopAndDestroy(parse);
		if (match)
			{
			return ETrue;
			}
		}
	return EFalse;
	}

TBool CImgDisplayMimeTypeRecognizer::CBody::GetTypeByFileDataL(const TDesC8& aImageData, TDes8& aMimeType)
	{
	aMimeType.SetLength(0);
	for (TInt i=iPluginArray.Count(); i--;)
		{
		const CImplementationInformation& impData = *(iPluginArray[i]);
		TInt bytesMatched=0;
		TBool match=CImageDisplayResolverUtils::Match(aImageData, impData.DataType(), bytesMatched);
		if (match && bytesMatched==impData.DataType().Length())
			{
			COpaqueDataParse* parse = COpaqueDataParse::NewLC(impData.OpaqueData());
			parse->EnsureMIMETypesReadL();
			aMimeType.Copy(parse->MIMEType(0)); // we always use the FIRST MIME type
			CleanupStack::PopAndDestroy(parse);
			if (aMimeType.Length()==0)
				{
				// we've got plug-in with empty MIME type i.e. generic one, so
				// it doesn't have MIME type and can't be used to recognize data
				continue;
				}
			return ETrue;
			}
		}
	return EFalse;
	}





