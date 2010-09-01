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

#include <icl/imagetransformplugin.h>
#include "TestTransformer.h"
#include "TestTransformerExt.h"


// Test transformer.

CTestTransformer* CTestTransformer::NewL()
	{
	CTestTransformer* self = new(ELeave) CTestTransformer();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CTestTransformer::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	iListener = CActiveListener::NewL(*this);
	}

CTestTransformer::CTestTransformer()
	{
	}


void CTestTransformer::Cleanup()
	{
	iState = EIdle;
	delete iImageDecoder;
	iImageDecoder = NULL;
	delete iScaler;
	iScaler = NULL;
	delete iImageEncoder;
	iImageEncoder = NULL;

	delete iSourceBitmap;
	iSourceBitmap = NULL;
	delete iDestBitmap;
	iDestBitmap = NULL;
	delete iExtension;
	iExtension = NULL;
	}

CTestTransformer::~CTestTransformer()
	{
	Cleanup();
	if (iListener != NULL && iListener->IsActive())
		iListener->Cancel();
	delete iListener;
	iFs.Close();
	}

void CTestTransformer::OpenL()
	{
	ASSERT (iState == EIdle);

	Cleanup();

	iIsSourceFilename = SourceIsFilename();
	iIsDestFilename = DestIsFilename();
	iIsSourceData = SourceIsData();
	iIsDestData = DestIsData();

	// get the source MIME type and/or image type (if specified)
	iSourceMimeType = &SourceMimeType();
	iSourceImageType = SourceImageType();
	iSourceImageSubType = SourceImageSubType();

	// for now, the destination MIME & image type are the same as the source
	iDestMimeType = iSourceMimeType;
	iDestImageType = iSourceImageType;
	iDestImageSubType = iSourceImageSubType;

	// get the size of the destination
	iDestSizeInPixels = DestinationSizeInPixels();

	// get whether to maintain the aspect ratio
	iMaintainAspectRatio = MaintainAspectRatio();

	// get whether to maintain the image data
	iPreserveImageData = PreserveImageData();

	//get options, currently no use for this
	iOptions = Options();
	
	//get source rect
	SourceRect(iSourceRect);

	// create the decoder
	if (iIsSourceFilename)
		{
		iSourceFilename.Set(SourceFilename());

		if (*iSourceMimeType != KNullDesC8)
			{
			iImageDecoder = CImageDecoder::FileNewL(iFs, iSourceFilename, *iSourceMimeType);
			}
		else
			{
			iImageDecoder = CImageDecoder::FileNewL(iFs, iSourceFilename);
			}
		}
	else if (iIsSourceData)
		{
		iSourceData.Set(SourceData());

		if (*iSourceMimeType != KNullDesC8)
			{
			iImageDecoder = CImageDecoder::DataNewL(iFs, *iSourceMimeType, iSourceData);
			}
		else
			{
			iImageDecoder = CImageDecoder::DataNewL(iFs, iSourceData);
			}
		}
	else
		{
		//currently only options for source type are filename and data
		User::Invariant();
		}

	// get the size of the image
	const TFrameInfo& frameInfo = iImageDecoder->FrameInfo(0);
	TBool color = (frameInfo.iFlags & TFrameInfo::EColor)?ETrue:EFalse;
	iSourceSizeInPixels = frameInfo.iOverallSizeInPixels;

	
	// set the destination type from the source type
	iImageDecoder->ImageType(0, iDestImageType, iDestImageSubType);


	// Create the source bitmap
	iSourceBitmap = new(ELeave) CFbsBitmap;
	User::LeaveIfError(iSourceBitmap->Create(iSourceSizeInPixels, color?EColor16M:EGray256));
		
	// create the scaler
	delete iScaler; iScaler = NULL;
	iScaler = CBitmapScaler::NewL();

	// Create the destination bitmap
	iDestBitmap = new(ELeave) CFbsBitmap;
	User::LeaveIfError(iDestBitmap->Create(iDestSizeInPixels, color?EColor16M:EGray256));
	
	if (iIsDestFilename)
		{
		iDestFilename.Set(DestFilename());
		if (*iDestMimeType == KNullDesC8)
			{
			iImageEncoder = CImageEncoder::FileNewL(
				iFs, iDestFilename, CImageEncoder::EOptionNone, iDestImageType, iDestImageSubType);
			}
		else
			{
			iImageEncoder = CImageEncoder::FileNewL(iFs, iDestFilename, *iDestMimeType);
			}
		}
	else if (iIsDestData)
		{
		iDestData = &DestData();
		if (*iDestMimeType == KNullDesC8)
			{
			iImageEncoder = CImageEncoder::DataNewL(
				*iDestData, CImageEncoder::EOptionNone, iDestImageType, iDestImageSubType);
			}
		else
			{
			iImageEncoder = CImageEncoder::DataNewL(*iDestData, *iDestMimeType);
			}
		}
	else
		{
		//currently only options for destination type are filename and data
		User::Invariant();
		}

	while(!iImageDecoder->IsImageHeaderProcessingComplete())
		{
		iImageDecoder->ContinueProcessingHeaderL();
		}
	iExtension = CTestTransformerExtension::NewL(this);
	}

void CTestTransformer::Transform(TRequestStatus& aStatus)
	{
	iClientStatus = &aStatus;

	iDestSize = DestinationSizeInPixels();

	// start by decoding the image to a bitmap
	iState = EDecodingImage;
	iListener->InitialiseActiveListener();
	iImageDecoder->Convert(&iListener->iStatus, *iSourceBitmap);
		
	}

void CTestTransformer::CancelTransform()
	{
	if (iImageDecoder)
		iImageDecoder->Cancel();

	if (iScaler)
		iScaler->Cancel();

	if (iImageEncoder)
		iImageEncoder->Cancel();

	if (iClientStatus != NULL && *iClientStatus == KRequestPending)
		{
		ASSERT (iState != EIdle);
		User::RequestComplete(iClientStatus, KErrCancel);
		}

	Cleanup();
	}

CImageTransformPluginExtension* CTestTransformer::Extension() const
	{
	return iExtension;
	}

void CTestTransformer::Complete(TInt aError)
	{
	if (aError != KErrNone)
		{
		User::RequestComplete(iClientStatus, aError);
		Cleanup();
		return;
		}

	switch (iState)
		{
		case EIdle:
			break;
		case EDecodingImage:
			delete iImageDecoder; iImageDecoder = NULL;
			ASSERT(iScaler);
			iState = EScalingBitmap;
			iListener->InitialiseActiveListener();
			iScaler->Scale(&iListener->iStatus, *iSourceBitmap, *iDestBitmap, iMaintainAspectRatio);
			break;

		case EScalingBitmap:
			delete iScaler; iScaler = NULL;
			iState = EEncodingImage;
			iListener->InitialiseActiveListener();
			ASSERT(iImageEncoder);
			iImageEncoder->Convert(&iListener->iStatus,  *iDestBitmap);
			break;

		case EEncodingImage:
			delete iImageEncoder; iImageEncoder = NULL;
			User::RequestComplete(iClientStatus, KErrNone);
			iState = EIdle;
			break;
		}
	}

TBool CTestTransformer::GetPreserveImageData() const
	{
	return iPreserveImageData;
	}


//CActiveListener

/*
NewL
*/
CActiveListener* CActiveListener::NewL(MListenerCallback &aListenerCallback)
	{
	CActiveListener* self = new (ELeave) CActiveListener(aListenerCallback);
	return self;
	}

/*
CActiveListener
*/
CActiveListener::CActiveListener(MListenerCallback &aListenerCallback) :
	CActive(CActive::EPriorityIdle), iListenerCallback(aListenerCallback)
	{
	CActiveScheduler::Add(this);
	}

/*
InitialiseActiveListener
*/
void CActiveListener::InitialiseActiveListener()
	{ 
    iStatus = KRequestPending; 
	SetActive(); 
	}

/*
RunL
*/
void CActiveListener::RunL()
	{
	TInt error = iStatus.Int();
	iListenerCallback.Complete(error);
	}

/*
  DoCancel()
  */
void CActiveListener::DoCancel()
	{
	}

