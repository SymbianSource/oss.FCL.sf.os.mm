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

#include "ExifJpegTransform.h"
#include "icl/ExifJpegTransformExtension.h"

#include "exiftransformdataaccessorimpl.h"
#include "ExifEditUtility.h"
#include <icl/icl_uids.hrh>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <icl/icl_uids_const.hrh>
#include <icl/icl_uids_def.hrh>
#endif



CExifJpegTransform* CExifJpegTransform::NewL()
	{
	CExifJpegTransform* self = new (ELeave) CExifJpegTransform;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	
	return self;
	}

CExifJpegTransform::~CExifJpegTransform()
	{
	delete iExtension;
	delete iExifEdit;
	
	//Can not simply delete iDest/iSource since all we have are M-class pointers.
	if (iDest)
		{
		iDest->Destroy();
		iDest = NULL;
		}

	if (iSource)
		{
		iSource->Destroy();
		iSource = NULL;
		}
				
	iFs.Close();
	}


CExifJpegTransform::CExifJpegTransform()
	{
	
	}
	
void CExifJpegTransform::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	}

MExifMetadata* CExifJpegTransform::ExifMetadata() const
	{
	if(iExifEdit)
		{
		return iExifEdit->ExifMetadata();	
		}
	return NULL;
	}


void CExifJpegTransform::Transform(TRequestStatus& aStatus)
	{
	TBool encodeThumbnail=EFalse;
	if((Options()&CImageTransform::EThumbnail)==CImageTransform::EThumbnail)
		{
		encodeThumbnail=ETrue;		
		}
	
	TRAPD(err, iExifEdit->WriteDestL(encodeThumbnail, DestinationSizeInPixels(), PreserveImageData(), MaintainAspectRatio(), &aStatus));
	if(err!=KErrNone)
		{
		aStatus=KRequestPending;
		TRequestStatus* status=&aStatus;
		User::RequestComplete(status, err);
		}
	}


void CExifJpegTransform::OpenL()
	{		
	if (SourceIsFilename())
		{
		iSource = new (ELeave) CExifFileSource(SourceFilename(), iFs);
		}
	else if (SourceIsData())
		{
		iSource = new (ELeave) CExifDescSource(SourceData(), iFs);
		}
	else
		{
		User::Leave(KErrNotSupported);
		}
		
	if (DestIsFilename())
		{
		iDest = new (ELeave) CExifFileDest(DestFilename(), iFs);
		}
	else if (DestIsData())
		{
		iDest = new (ELeave) CExifDescDest(DestData());
		}
	else
		{
		User::Leave(KErrNotSupported);
		}
	iExifEdit=CExifEditUtility::NewL(iSource, iDest, Options() & CImageTransform::EIgnoreExifMetadataProcessing);	
	iExifEdit->ReadSourceL();
	iExtension=(CImageTransformPluginExtension*)CJpegTransformExtension::NewL(this);	
	}

	
void CExifJpegTransform::CancelTransform()
	{
	iExifEdit->Cancel();
	}

CImageTransformPluginExtension* CExifJpegTransform::Extension() const
	{
	return iExtension;
	}
