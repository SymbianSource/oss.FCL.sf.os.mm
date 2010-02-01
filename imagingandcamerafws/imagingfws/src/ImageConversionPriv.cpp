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

#include "ImageClientMain.h"
#include "ImageConversionPriv.h"
#include "icl/ImagePlugin.h"
#include "icl/ImageConstruct.h"
#include "ImageConversion.h"



const TInt KMaxDstBufferSize = 4096;

/* IMAGE DECODER PRIVATE */
CImageDecoderPriv::CImageDecoderPriv(CImageDecodeConstruct* aConstruct, MImageDecoderPrivSupport* aSupport)
	: CActive(CActive::EPriorityIdle), iImageType(CImageDecoder::EImageTypeMain), iConstruct(aConstruct), iSupport(aSupport), iReadOnly(EFalse), iUseBufferReadOptimization(ETrue)
	{
	ASSERT(aConstruct != NULL);
	ASSERT(aSupport != NULL);
	CActiveScheduler::Add(this);
	}

void CImageDecoderPriv::SetFileL(RFs& aFs, const TDesC& aSourceFilename, const CImageDecoder::TOptions aOptions)
	{
	iFs = &aFs;
	iOptions = aOptions;
	iImageParameterData.SetFilenamePtr(&aSourceFilename);
	delete iContent;
	iContent = NULL;
	delete iData;
	iData = NULL;
	iReadOnly=ETrue;
	TRAPD(err, iContent = CContent::NewL(*iImageParameterData.SourceFilenamePtr(), EContentShareReadOnly));
	if (err!= KErrNone && err != KErrNoMemory)
		{
		iReadOnly=EFalse;
		iContent = CContent::NewL(*iImageParameterData.SourceFilenamePtr(), EContentShareReadWrite);
		}
	User::LeaveIfNull(iContent);
	iData = iContent->OpenContentL(iIntent, (iUniqueId) ? static_cast<TDesC&>(*iUniqueId) : KDefaultContentObject());
	
	//Try to find if content is DRM
    TInt value = -1;
    TInt getAttrResult =iContent->GetAttribute(EIsProtected, value);
    if(getAttrResult != KErrNone || value!=0)
        {
        //either we had an error from getAttribute or  EIsProtected is not set to 0 (false)
        //Could be DRM content, don't apply iUseBufferReadOptimization
        iUseBufferReadOptimization = EFalse;
        }   
    
	iCachedDataSize = -1;
    }
	
void CImageDecoderPriv::SetFileL(RFile& aFile, const CImageDecoder::TOptions aOptions)
	{
	iOptions = aOptions;
	User::LeaveIfError(aFile.Name(iFileName));
	iImageParameterData.SetFilenamePtr(&iFileName);
	delete iContent;
	iContent = NULL;
	delete iData;
	iData = NULL;
	iContent = CContent::NewL(aFile);
	iData = iContent->OpenContentL(iIntent, (iUniqueId) ? static_cast<TDesC&>(*iUniqueId) : KDefaultContentObject());
	
	//Try to find if content is DRM
    TInt value = -1;
    TInt getAttrResult =iContent->GetAttribute(EIsProtected, value);
    if(getAttrResult != KErrNone || value!=0)
        {
        //either we had an error from getAttribute or EIsProtected is not set to 0 (false)
        //Could be DRM content, don't apply iUseBufferReadOptimization
        iUseBufferReadOptimization = EFalse;
        }   
	
	iCachedDataSize = -1;        
	}

void CImageDecoderPriv::SetDataL(RFs& aFs, const TDesC8& aSourceData, const CImageDecoder::TOptions aOptions)
	{
	iFs = &aFs;
	iOptions = aOptions;
	iImageParameterData.SetDataPtr(&aSourceData);
	}

CImageDecoderPriv* CImageDecoderPriv::NewL(CImageDecodeConstruct* aConstruct, MImageDecoderPrivSupport* aSupport)
	{
	CImageDecoderPriv* self = new (ELeave) CImageDecoderPriv(aConstruct, aSupport);
	return self;
	}

CImageDecoderPriv::~CImageDecoderPriv()
	{
	ASSERT(!IsActive());
	if (iPlugin)
		iPlugin->Cleanup();
	iFrameInfo.ResetAndDestroy();
	iFrameData.ResetAndDestroy();
	delete iImageReadCodec; // Created in ScanDataL()
	delete iData;
	delete iContent;
	delete iPlugin;
	delete iUniqueId;
	}

void CImageDecoderPriv::Cleanup()
	{
	delete iReadBuffer; iReadBuffer = NULL;	
	}

void CImageDecoderPriv::CreatePluginL()
	{
	iPlugin = iConstruct->NewPluginL();
	iPlugin->iProperties = this;
	}

/**
@see CImageDecoder::SetImageTypeL
*/
void CImageDecoderPriv::SetImageTypeL(TInt aImageType)
	{
	if (aImageType == iImageType)
		{
		return;
		}

	if ( (aImageType == CImageDecoder::EImageTypeThumbnail) &&
		 (iThumbnailData == NULL) )
		{
		User::Leave(KErrNotFound);
		}
	
	TInt prevImageType = iImageType;	
	iImageType = aImageType;
	
	// Delete the old read codec - a new one will be instantiated by the 
	// plugin on notification of Image Type change.
	delete iImageReadCodec;
	iImageReadCodec = NULL;
	iCurrentFrame = 0;
	iPosition = 0;
	TRAPD(err,iPlugin->NotifyImageTypeChangeL(aImageType));
	if(err != KErrNone)
		{
		delete iImageReadCodec;
		iImageReadCodec = NULL;
		// Try to switch back to the main image
		iImageType = prevImageType;
		TRAPD(err2,iPlugin->NotifyImageTypeChangeL(prevImageType));
		if (err2 != KErrNone)
			{
			// Switching back to the main image type shouldn't fail as corrupt as it has been done once before 
			// when creating the decoder.
			__ASSERT_DEBUG( (err2 != KErrCorrupt), Panic( EInvalidFunctionLeave ) );
			User::Leave(err2);
			}
		// We report the thumbnail problem to the application
		User::Leave(err);
		}
	}

void CImageDecoderPriv::SetThumbnailData(HBufC8* aThumbnailData)
	{
	// The buffer containing the thumbnail data is owned by the plugin
	iThumbnailData = aThumbnailData;
	}

/*
Disable optimization of this function by the VC compiler.
This function suffers from the known behaviour of setjmp()/longjmp()
used by the TRAP() harness in that register variables can be 'stomped'
by the longjmp/leave process. 
*/
#ifdef __VC32__
#pragma optimize("",off)
#endif  // __VC32__
void CImageDecoderPriv::Convert(RThread& aRequestThread, TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, TInt aFrameNumber)
	{
	if (IsActive())
		Panic(EConvertCalledWhileBusy);
	

	iRequestThread = &aRequestThread;
	iConvStatus = aRequestStatus;
 	*iConvStatus = KRequestPending;

	iCurrentFrame = aFrameNumber;
	if (iCurrentFrame < 0 || iCurrentFrame >= iFrameInfo.Count())
		{
		RequestComplete(KErrArgument);
		return;
		}
	
	if (aDestination.Handle() == 0)
		Panic(ENoDestinationBitmap);
	
   if(aDestination.ExtendedBitmapType()!=KNullUid)
        {
        RequestComplete(KErrNotSupported);
        return;
        }
	
	iDestination = &aDestination;
	iDestinationHandle = iDestination->Handle();
	iDestinationSize = iDestination->SizeInPixels();
	iDestinationMask = NULL;
	TRAPD(error, iPlugin->InitConvertL());

	if (error!=KErrNone)
		RequestComplete(error);
	else
		SelfComplete(KErrNone);
	}
#ifdef __VC32__
#pragma optimize("",on)
#endif  // __VC32__

void CImageDecoderPriv::Convert(RThread& aRequestThread, TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, CFbsBitmap& aDestinationMask, TInt aFrameNumber)
	{	
	if (IsActive())
		Panic(EConvertCalledWhileBusy);

	iRequestThread = &aRequestThread;
	iConvStatus = aRequestStatus;
	*iConvStatus = KRequestPending;

	iCurrentFrame = aFrameNumber;
	if (iCurrentFrame < 0 || iCurrentFrame >= iFrameInfo.Count())
		{
		RequestComplete(KErrArgument);
		return;
		}
	
	if (aDestination.Handle() == 0)
		Panic(ENoDestinationBitmap);
	
   if(aDestination.ExtendedBitmapType()!=KNullUid || (aDestinationMask.Handle()!=0 && aDestinationMask.ExtendedBitmapType()!=KNullUid))
        {
        RequestComplete(KErrNotSupported);
        return;
        }

	iDestination = &aDestination;
	iDestinationHandle = iDestination->Handle();
	iDestinationSize = iDestination->SizeInPixels();

	if (aDestinationMask.Handle() == 0)
		Panic(ENoBitmapMask);
	else
		{
		// if we have an alpha channel mask, insist an EGray256 mask bitmap
		// else allow either an EGray2 or an EGray256 one
		// ie. for compatability allow an EGray256 mask bitmap, even if we only have binary data
		TDisplayMode maskDisplayMode = aDestinationMask.DisplayMode();
		const TFrameInfo& frameInfo = FrameInfo(iCurrentFrame);
		if (!(maskDisplayMode==EGray256 || 
			(maskDisplayMode==EGray2 && !(frameInfo.iFlags & TFrameInfo::EAlphaChannel))))
			{
			RequestComplete(KErrArgument);
			return;
			}
		// furthermore, if we have a mask, enforce is same size as destination
		if (iDestinationSize != aDestinationMask.SizeInPixels())
			{
			RequestComplete(KErrArgument);
			return;
			}
		}

	iDestinationMask = &aDestinationMask;
	iDestinationMaskHandle = iDestinationMask->Handle();
	
	TRAPD(error, iPlugin->InitConvertL());

	if (error!=KErrNone)
		RequestComplete(error);
	else
		SelfComplete(KErrNone);
	}

void CImageDecoderPriv::ContinueConvert(RThread& aRequestThread, TRequestStatus* aRequestStatus)
	{
	// NB Not busy and iCurrentProcessingData==true -> we previously stopped on an underflow
	// if not true we must have stopped for some other reason and ContinueConvert is illegal

	if (IsActive())
		Panic(EConvertCalledWhileBusy);

	if (!iCurrentlyProcessingData)
		Panic(EIllegalContinueConvert);

	ASSERT(iPlugin->ValidDestination()); // if legal we should have an iDestination. Whether iMask depends.

	if (iDestination->Handle() != iDestinationHandle)
		Panic(EDifferentDestinationBitmap);

	if (iDestinationMask && (iDestinationMask->Handle() != iDestinationMaskHandle))
		Panic(EDifferentDestinationMask);

	if (iDestination->SizeInPixels() != iDestinationSize)
		Panic(EModifiedDestination);

	iRequestThread = &aRequestThread;
	iConvStatus = aRequestStatus;
	*iConvStatus = KRequestPending;

	SelfComplete(KErrNone);
	return;
	}


TInt CImageDecoderPriv::DataSizeL()
    {
    TInt dataSize = iCachedDataSize;
    if (dataSize == -1)
        {
        iData->DataSizeL(dataSize);
        if(iReadOnly)
             iCachedDataSize = dataSize;
        }
    
    return dataSize;
    }

void CImageDecoderPriv::PrepareForProcessFrameL()
	{
	TInt localSrcLength;
	TBool srcHasGrown = EFalse;

	if (iImageParameterData.IsFilename())
		{
		ASSERT(iData);
		
        if(iUseBufferReadOptimization)
            {
            localSrcLength = DataSizeL();
            }
        else
            {
            iData->DataSizeL(localSrcLength);
            }
		}
	else
		{
		localSrcLength = iImageParameterData.SourceDataPtr()->Length();
		}

	if (localSrcLength > iSrcLength)
		srcHasGrown = ETrue;

	iSrcLength = localSrcLength;
	if ((iSrcDes.Length() == 0) || srcHasGrown)
		{
		// Fetch next buffer.
		// Get next block size.
		iCurrentBlockLength = iBlockLength;
		if ((iCurrentBlockLength + iPosition) > iDataLength)
			{
			// If current position + read buffer length > Data length then
			// iCurrentBlockLength = what ever's left in iDataLength
			iCurrentBlockLength = Max(0, iDataLength - iPosition);
			}

		// Get the next block of data
		TPtrC8 ptr;
		ReadDataL((iPosition + iStartPosition), ptr, iCurrentBlockLength);

		// Set iSrcDes to point at next block in the source data.
		iSrcDes.Set(ptr);
		iCurrentBlockLength = iSrcDes.Length();
		}

	iOldSrcDesLength = iSrcDes.Length();
	}


 
/**
* Issues a read operation by sending the start position
* along with the destination descriptor and size.
*
* When this function is used to read data from a file, it
* can send the start position and length in one IPC operation,
* and so avoid having to issue a seek first.
*
* Because ContentAccess::CData does not support a synchronous
* version of this API, this function takes care of waiting for
* the operation to complete, and leaves with any error code.
* This simplifies the calling code.
*
* @param   aPosition       Position in source data to read from.
* @param   aDest           The data is written into this descriptor.
* @param   aSize           Amount of data to read in bytes.
* @leave                   Any Symbian OS error code.
*/
void CImageDecoderPriv::SeekReadL(TInt aPosition, TDes8& aDest, TInt aSize)
    {
    #ifdef __WINS__
        RDebug::Printf("sr,%d,%d", aPosition, aSize);
    #endif
        
    TRequestStatus rs;
    TInt result=iData->Read(aPosition, aDest, aSize, rs);
    User::LeaveIfError(result);
    User::WaitForRequest(rs);
    User::LeaveIfError(rs.Int());
    }


void CImageDecoderPriv::DoConvert()
	{
	TRAPD(errCode, PrepareForProcessFrameL());

	if (errCode!=KErrNone)
		{
		RequestComplete(errCode);
		return;
		}

	TFrameState codecState = EFrameIncomplete; 
	if (iCurrentBlockLength)
		TRAP(errCode, codecState = iImageReadCodec->ProcessFrameL(iSrcDes));
	
	HandleProcessFrameResult(errCode, codecState);
	}

void CImageDecoderPriv::HandleProcessFrameResult(TInt aErrCode, TFrameState aCodecState)
	{
	if (aErrCode!=KErrNone)
		{
		ASSERT(aErrCode != KErrUnderflow);
		iCurrentlyProcessingData = EFalse;
		RequestComplete(aErrCode);
		return;
		}

	TInt newSrcDesLength = iSrcDes.Length();
	iPosition += iOldSrcDesLength - newSrcDesLength;

	if (aCodecState == EFrameIncompleteRepositionRequest)
		{
		TInt length   = iBlockLength;
		TInt position = iPosition + iStartPosition;
		iImageReadCodec->GetNewDataPosition(position, length);
		if (position<0)
			{
			iCurrentlyProcessingData = EFalse;
			RequestComplete(KErrCorrupt);
			return;
			}

		iPosition = position-iStartPosition;
		}
	
	if (aCodecState == EBlockComplete)	// Block decoded successfully
		{
		iCurrentlyProcessingData = EFalse; // To avoid call of ContinueConvert()
		RequestComplete(KErrNone);
		return;
		}
	
	if (aCodecState == EFrameComplete)	// Frame decoded successfully
		{
		iImageReadCodec->Complete();
		iCurrentlyProcessingData = EFalse;

		RequestComplete(KErrNone);
		return;
		}
	else if (newSrcDesLength == iOldSrcDesLength && iCurrentlyProcessingData) // Needs more data than is currently present in the buffer to make progress
		{
		//call of ContinueConvert() only
		 //The following method call is required so that codecs can get the data already decoded can be displayed
		iImageReadCodec->Complete();
		RequestComplete(KErrUnderflow);
		return;
		}

	// Each Plug-in has to trigger the active object as it needs RunL called again.
	iSrcDes.SetLength(0);
	SelfComplete(KErrNone);
	}

TBufPtr8& CImageDecoderPriv::SourceData()
	{
	return iSrcDes;
	}


void CImageDecoderPriv::RunL()
	{
	ASSERT(iStatus==KErrNone); // we don't handle errors at this point. Should only get here on KErrNone
	iPlugin->DoConvert();
	}

void CImageDecoderPriv::RequestComplete(TInt aReason)
	{
	ASSERT(iPlugin);
	iPlugin->NotifyComplete();
	ASSERT(iRequestThread);

	if (iData && aReason == KErrNone && !iIntentHasBeenExecuted)
		{
		TInt err = iData->ExecuteIntent(iIntent);
		if(err == KErrNone)
			{
			//Intent has been executed. Ensure it is not executed again for this image.
			iIntentHasBeenExecuted = ETrue;
			}
		else
			{
			aReason = err;
			}
		}

	if (iConvStatus && (*iConvStatus == KRequestPending) )
		{
		iRequestThread->RequestComplete(iConvStatus, aReason);
		}
	}

void CImageDecoderPriv::DoCancel()
	{
	iPlugin->Cleanup();

	// if SetSelfPending() has been called but not SelfComplete()
	// (i.e. the AO is active but there's still an outstanding request)
	// then we need to complete the request
	if (IsActive() && iStatus == KRequestPending)
		SelfComplete(KErrCancel);

	RequestComplete(KErrCancel);
	}

void CImageDecoderPriv::SelfComplete(TInt aReason)
	{
	if (!IsActive())	// may already be active if SetSelfPending() called
		SetActive();
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, aReason);
	}

void CImageDecoderPriv::SetSelfPending()
	{
	iStatus = KRequestPending;
	SetActive();
	}

void CImageDecoderPriv::ContinueProcessingHeaderL()
	{
	iCurrentFrame = iFrameInfo.Count();
	iProcessingFrameInfo = EFalse;

	if (iCurrentFrame)
		{
		TInt previousFrame = iCurrentFrame-1;
		iPosition = iFrameInfo[previousFrame]->iCurrentDataOffset;
		if (iFrameInfo[previousFrame]->iCurrentFrameState == TFrameInfo::EFrameInfoProcessingFrame)
			{
			iCurrentFrame--;
			iProcessingFrameInfo = ETrue;
			}
		}
		else
			iPosition = 0;

	TInt error;
	if (iImageReadCodec)
		{
		TRAP(error,iPlugin->ReadFrameHeadersL());
		}
	else
		{
		TRAP(error,iPlugin->ScanDataL());
		}

	if (error!=KErrNone && error!=KErrUnderflow) // ignore underflow at this point
		User::Leave(error);
	}

void CImageDecoderPriv::CreateFrameInfoL()
	{
	// Create a new instance of TFrameInfo and initialise it.
	// Insert it into the frame info array and store the frame offset.
	// Mark the frame as being processed.
	TFrameInfo* newFrameInfo = new(ELeave) TFrameInfo(iImageInfo);

	CleanupStack::PushL(newFrameInfo); // push in case insert fails
	User::LeaveIfError(iFrameInfo.Insert(newFrameInfo, iCurrentFrame));
	CleanupStack::Pop(); // newFrameInfo
				
	iFrameInfo[iCurrentFrame]->iFrameDataOffset = iNextFrameOffset;
	iProcessingFrameInfo = EFalse;

	CFrameImageData* newFrameData = CFrameImageData::NewL(iImageData);
	CleanupStack::PushL(newFrameData);
				
	User::LeaveIfError(iFrameData.Insert(newFrameData, iCurrentFrame));
	CleanupStack::Pop(); // newFrameData

	iImageReadCodec->SetCurrentFrame(iCurrentFrame);
	iImageReadCodec->InitFrameHeader(*iFrameInfo[iCurrentFrame], *iFrameData[iCurrentFrame]);
	}

void CImageDecoderPriv::DeleteFrameInfoL()
	{
	// If process data needs another block but we don't have one because
	// this block is incomplete.
	if (iFrameInfo.Count()>1)
		{
		// Delete the new frame as it is incomplete.
		iSupport->Lock();
		delete iFrameInfo[iCurrentFrame];
		iFrameInfo.Remove(iCurrentFrame);

		delete iFrameData[iCurrentFrame];
		iFrameData.Remove(iCurrentFrame);

		iHaveCompleteImageHeaders = EFalse;
		iSupport->Unlock();
		iSupport->SaveFrameCount(FrameCount());
		return;
		}
	else //may need to do some more clean-up
		{
		// No headers at all, so just leave.
		iSupport->Lock();
		iFrameInfo.ResetAndDestroy();
		iFrameData.ResetAndDestroy();
		iSupport->Unlock();
		iSupport->SaveFrameCount(FrameCount());
		User::Leave(KErrUnderflow);
		}
	}

void CImageDecoderPriv::ReadFrameHeadersL()
	{
	ASSERT(iImageReadCodec); // further down, assumes we can call ProcessHeaderL on the codec

	TFrameState codecState;
	TFrameInfo::TFrameInfoState frameState;

	iNextFrameOffset = iPosition;
	iCurrentlyProcessingData = EFalse;
	iBlockLength = iPlugin->FrameHeaderBlockSize(iCurrentFrame);

	do
		{ 
		iSrcLength = iBlockLength;
		if ((iSrcLength + iPosition) > iDataLength)
			{
			// If current position + read buffer length > Data length then
			// srcLength = what ever's left in iDataLength
			iSrcLength = Max(0, iDataLength - iPosition);
			}

		TPtrC8 ptr;
		ReadDataL((iPosition + iStartPosition), ptr, iSrcLength);
		iSrcDes.Set(ptr);
		iSrcLength = iSrcDes.Length();

		TInt err;
		if (!iProcessingFrameInfo)
			{
			iSupport->Lock();

			//Do not leave between lock and unlock
			TRAP(err, CreateFrameInfoL());

			iSupport->Unlock();
			if (err != KErrNone)
				{	// In case of leave, we need to reset the pointer arrays
				iSupport->Lock();
				iFrameInfo.ResetAndDestroy();
				iFrameData.ResetAndDestroy();
				iSupport->Unlock();
				iSupport->SaveFrameCount(FrameCount());
				User::Leave(err);
				}

			iSupport->SaveFrameCount(FrameCount());
			}

		// Process Header for one frame.
		codecState = EFrameIncomplete;
		err = KErrUnderflow;
		if (iSrcLength)
			TRAP(err, codecState = iImageReadCodec->ProcessFrameHeaderL(iSrcDes));

		if ((err != KErrNone) && (err != KErrUnderflow))
			User::Leave(err);
		
		TInt newSrcLength = iSrcDes.Length();
		frameState = iFrameInfo[iCurrentFrame]->iCurrentFrameState;
		if ((err == KErrUnderflow) || ((codecState == EFrameIncomplete) && (iSrcLength == newSrcLength)))
			{
			if ((frameState == TFrameInfo::EFrameInfoUninitialised) || (frameState == TFrameInfo::EFrameInfoProcessingFrameHeader))
				DeleteFrameInfoL();
			return;
			}

		if (codecState == EFrameIncompleteRepositionRequest)
			{
			TInt length   = iBlockLength;
			TInt position = iPosition+iStartPosition;
			iImageReadCodec->GetNewDataPosition(position, length);
			if (position<0)
				User::Leave(KErrCorrupt);

			iPosition = position-iStartPosition;
			}
		else
			iPosition += iSrcLength - newSrcLength;

		iNextFrameOffset = iPosition; // Store next frame header offset.
		iProcessingFrameInfo = (frameState == TFrameInfo::EFrameInfoProcessingFrameHeader) || (frameState == TFrameInfo::EFrameInfoProcessingFrame);
		if ((frameState == TFrameInfo::EFrameInfoProcessingFrame) || (frameState == TFrameInfo::EFrameInfoProcessingComplete))
			{
			iFrameInfo[iCurrentFrame]->iCurrentDataOffset = iPosition;

			if (frameState == TFrameInfo::EFrameInfoProcessingComplete)
				iCurrentFrame++;
			}
		}
	while ((codecState == EFrameIncomplete) || (codecState == EFrameIncompleteRepositionRequest));

	// Frame header read complete
	if(frameState == TFrameInfo::EFrameInfoProcessingFrameHeader)
		DeleteFrameInfoL();

	iHaveCompleteImageHeaders = ETrue;
	}

void CImageDecoderPriv::InitConvertL()
	{
	ASSERT((iCurrentFrame >= 0) && (iCurrentFrame < iFrameInfo.Count()));

	// Set position to start of frame.
	iPosition = iFrameInfo[iCurrentFrame]->iFrameDataOffset;

	// Re-set conversion variables.
	iSrcLength = 0;

	// Set flag to say that we are now converting an image.
	iCurrentlyProcessingData = ETrue;

	// Re-initialise block length and codec
	iBlockLength = iPlugin->FrameBlockSize(iCurrentFrame);

	TBool disableErrorDiffusion = (iOptions & CImageDecoder::EOptionNoDither);
	iImageReadCodec->SetCurrentFrame(iCurrentFrame);
	iImageReadCodec->InitFrameL(*iFrameInfo[iCurrentFrame], *iFrameData[iCurrentFrame], disableErrorDiffusion,  *iDestination, iDestinationMask);

	}

void CImageDecoderPriv::RequestInitL(TInt aFrameNumber)
	{
	__ASSERT_ALWAYS(!IsActive(), Panic(EConvertCalledWhileBusy));
	__ASSERT_ALWAYS((aFrameNumber >= 0) && (aFrameNumber < iFrameInfo.Count()), User::Leave(KErrArgument));

	// Set current frame.
	iCurrentFrame = aFrameNumber;
	iImageReadCodec->SetCurrentFrame(iCurrentFrame);

	// Set position to start of frame.
	iPosition = iFrameInfo[iCurrentFrame]->iFrameDataOffset;

	// Re-set conversion variables.
	iSrcLength = 0;

	// Set flag to say that we are now converting an image.
	iCurrentlyProcessingData = ETrue;

	// Re-initialise block length and codec
	iBlockLength = iPlugin->FrameBlockSize(iCurrentFrame);
	}
	
const TFrameInfo& CImageDecoderPriv::FrameInfo(TInt aFrameNumber) const
	{
	// Return the frame info for a particular frame
	__ASSERT_ALWAYS((aFrameNumber >= 0) && (aFrameNumber < iFrameInfo.Count()), Panic(EFrameNumberOutOfRange));
	return *iFrameInfo[aFrameNumber];
	}

CFrameInfoStrings* CImageDecoderPriv::FrameInfoStringsLC(TInt aFrameNumber)
	{
	CFrameInfoStrings* frameInfoStrings;
	if (iFs == NULL)
		{
		RFs localRFs;
		CleanupClosePushL(localRFs);

		User::LeaveIfError(localRFs.Connect());
		frameInfoStrings = iPlugin->FrameInfoStringsL(localRFs, aFrameNumber);
		CleanupStack::PopAndDestroy(); // localRFs
		}
	else
		frameInfoStrings = iPlugin->FrameInfoStringsL(*iFs, aFrameNumber);

	CleanupStack::PushL(frameInfoStrings);
	return frameInfoStrings;
	}

void CImageDecoderPriv::ReadDataL(TInt aPosition, TPtrC8& aReadBuffer, TInt aLength)
	{
	ASSERT(aPosition >= 0); 
 	ASSERT(aLength >= 0);	

	TBool srcIsFile = ETrue;
	TInt  srcSize = 0;
	TInt  size = aLength;

	if (iImageType == CImageDecoder::EImageTypeThumbnail) 
		{
		ASSERT(iThumbnailData!=NULL);
		
		srcSize = iThumbnailData->Length();
		
		// if trying to start reading past end of file do nothing
		if(aPosition >= srcSize)
			{
			aReadBuffer.Set(KNullDesC8);
			return;
			}

		if((aPosition + size) > srcSize)
			{
			// there is not enough data left read what is left.
			size = srcSize - aPosition;
			}

		const TDesC8* srcDes = iThumbnailData;
		ASSERT(srcDes != NULL);			
		aReadBuffer.Set(srcDes->Mid(aPosition, size));
		iSrcDes.Set(aReadBuffer);		
		}
	else
		{
		
		if(iImageParameterData.IsFilename())
			{ // The source if a file
			ASSERT(iData); // The file has to be open
			if (iUseBufferReadOptimization)
                {
				srcSize = DataSizeL();
                }
			else
                {
				iData->DataSizeL(srcSize);
                }
			

			srcIsFile = ETrue;
			}
		else
			{ // The source is a descriptor
			srcSize = iImageParameterData.SourceDataPtr()->Length();
			srcIsFile = EFalse;
			}

		// if trying to start reading past end of file do nothing
		if(aPosition >= srcSize)
			{
			aReadBuffer.Set(KNullDesC8);
			return;
			}

		if((aPosition + size) > srcSize)
			{
			// there is not enough data left read what is left.
			size = srcSize - aPosition;
			}

		TBool bufferedRead = srcIsFile || MustUseBufferWithDescriptor();

		if(bufferedRead)
			{
			TBool allocNewBuf = EFalse;
         
			// read source into local buffer
			if(iReadBuffer == NULL)
				{
				iReadBuffer = HBufC8::NewMaxL(size);
				allocNewBuf = ETrue;
				}

			TPtr8 ptr(iReadBuffer->Des());
			//Grow iReadBuffer if necessary
			if(ptr.MaxLength() < size)
				{
				delete iReadBuffer; iReadBuffer = NULL;
				iReadBuffer = HBufC8::NewMaxL(size);
				ptr.Set(iReadBuffer->Des());
				allocNewBuf = ETrue;				     
				}

			if (srcIsFile)
				{
                if (iUseBufferReadOptimization)
                    {
                    // See if the end of the last read overlaps the start of the next read.
                    TInt extent = aPosition + size;
                    TBool overlaps =
                    // not newly-created buffer
                        ! allocNewBuf
                    // start position at or before start of last read
                        &&  iLastPos <= aPosition
                    // extent (end position) currently in buffer
                        &&  iLastExtent > aPosition && iLastExtent <= extent;
	               
                    if (overlaps)
                        {
                        // copy the overlap portion to the start of the read buffer
                        TInt overlapOffset = aPosition - iLastPos;
                        TInt overlapAmount = iLastExtent - aPosition;
                        ptr.Copy(ptr.Mid(overlapOffset, overlapAmount));
                                   
                        // read the excess from the file
                        // expand ptr, else MidTPtr will only use the current data
                        // after the overlap amount for its maximum length
                        ptr.SetMax();
                        TPtr8 excess(ptr.MidTPtr(overlapAmount));
                        SeekReadL(aPosition + overlapAmount, excess, size - overlapAmount);
                                   
                        ptr.SetLength(overlapAmount + excess.Length());
                        }

                    else
                        {
                        SeekReadL(aPosition, ptr, size);
                        }
   
                    iLastPos = aPosition;
                    iLastExtent = extent;
                    }
                else
                    {
                    User::LeaveIfError(iData->Seek(ESeekStart, aPosition));
                    User::LeaveIfError(iData->Read(ptr, size));
                    }
                
				}
			else
				{
				ASSERT(!srcIsFile && MustUseBufferWithDescriptor());
				const TDesC8* srcDes = iImageParameterData.SourceDataPtr();
				ASSERT(srcDes != NULL);			
				CopyBufferToDescriptor(ptr, *srcDes, aPosition, size); 
				}
			aReadBuffer.Set(iReadBuffer->Des());
			}
		else
			{//Read block from descriptor
			const TDesC8* srcDes = iImageParameterData.SourceDataPtr();
			ASSERT(srcDes != NULL);			
			aReadBuffer.Set(srcDes->Mid(aPosition, size));
			}
		iSrcDes.Set(aReadBuffer);
		}
	}
 
void CImageDecoderPriv::HandleNewlyOpenedImageL()
	{
	// Tell the plugin that the user wants mask generation.
	if (iOptions & CImageDecoder::EAllowGeneratedMask)
		iPlugin->EnableMaskGeneration();

	// call ScanDataL to read in the first header, at least.
	TRAPD(error,iPlugin->ScanDataL());

	// ignore underflow at this point only if the user has set the EOptionAllowZeroFrameOpen flag
	if (error == KErrUnderflow)
		if (iOptions & CImageDecoder::EOptionAllowZeroFrameOpen)
			return;
		else
			User::Leave(error);
		

	if (error!=KErrNone) 
		User::Leave(error);
	}

TUid CImageDecoderPriv::ImplementationUid() const
	{
	return iConstruct->ImplementationUid();
	}

/**
 * Passes on CustomSyncL to plugin
 *
 */

void CImageDecoderPriv::CustomSyncL(TInt aParam)
	{
	iPlugin->HandleCustomSyncL(aParam);
	}

/**
 * Plugin is interrogated with InitCustomAsyncL. If this works, then set internal Convert() cycle going
 *
 */

void CImageDecoderPriv::CustomAsync(RThread& aRequestThread, TRequestStatus* aRequestStatus, TInt aParam)
	{
	__ASSERT_ALWAYS(!IsActive(), Panic(EConvertCalledWhileBusy));

	TRAPD(error, iPlugin->InitCustomAsyncL(aParam));

	iRequestThread = &aRequestThread;
	iConvStatus = aRequestStatus;
	*iConvStatus = KRequestPending;

	if (error==KErrNone)
		SelfComplete(KErrNone);
	else
		RequestComplete(error);
	}

/**
 *  Indicate if running in a separate thread
 * 
 */

TBool CImageDecoderPriv::AmInThread() const
	{
	return iSupport->AmInThread();
	}

/**
 *  Indicate if a decode should abort early (ie. following a Cancel). Always false unless in a thread.
 * 
 */

TBool CImageDecoderPriv::ShouldAbort() const
	{
	return iSupport->ShouldAbort();
	}

void CImageDecoderPriv::SetIntent(ContentAccess::TIntent aIntent)
	{
	iIntent = aIntent;
	}

void CImageDecoderPriv::SetUniqueIdL(const TDesC& aUniqueId)
	{
	delete iUniqueId;
	iUniqueId = NULL;
	iUniqueId = aUniqueId.AllocL();
	}

TInt CImageDecoderPriv::SetAgentProperty(ContentAccess::TAgentProperty aProperty, TInt aValue)
	{
	return iContent->SetProperty(aProperty, aValue);
	}
	
TInt CImageDecoderPriv::ReductionFactor(const TSize& aOriginalSize, const TSize& aReducedSize) const
	{
	return iImageReadCodec->ReductionFactor(aOriginalSize, aReducedSize);
	}

TInt CImageDecoderPriv::ReducedSize(const TSize& aOriginalSize, TInt aReductionFactor, TSize& aReducedSize) const	
	{
	return iImageReadCodec->ReducedSize(aOriginalSize, aReductionFactor, aReducedSize);
	}
	
void CImageDecoderPriv::GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr)
	{
	iPlugin->GetExtensionL(aExtUid, aExtPtr);
	}
	
void CImageDecoderPriv::SetClippingRectL(const TRect* aClipRect)
	{
	iPlugin->SetClippingRectL(aClipRect);
	}
	
TInt CImageDecoderPriv::GetDestinationSize(TSize& aSize, TInt aFrameNumber)
	{
	return iPlugin->GetDestinationSize(aSize, aFrameNumber);
	}

void CImageDecoderPriv::RegisterClientRequestStatus(RThread& aRequestThread, TRequestStatus* aRequestStatus)
	{
	__ASSERT_ALWAYS(!IsActive(), Panic(EFwExtensionBusy));

	iRequestThread = &aRequestThread;
	iConvStatus = aRequestStatus;
	*iConvStatus = KRequestPending;
	}
	
void CImageDecoderPriv::StartActivity()
	{
	SetSelfPending();
	SelfComplete(KErrNone);
	}

/* IMAGE ENCODER PRIVATE*/
CImageEncoderPriv* CImageEncoderPriv::NewL(CImageEncodeConstruct* aConstruct, MImageEncoderPrivSupport* aSupport)
	{
	CImageEncoderPriv* self = new (ELeave) CImageEncoderPriv(aConstruct, aSupport);
	return self;
	}

void CImageEncoderPriv::SetFileL(RFs& aFs, const TDesC& aDestinationFilename, const CImageEncoder::TOptions aOptions)
	{
	iFs = &aFs;
	User::LeaveIfError(iFile.Replace(*iFs, aDestinationFilename, EFileWrite | EFileShareAny));
	iImageParameterData.SetFilenamePtr(&aDestinationFilename);
	iOptions = aOptions;
	
#if defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)	
	ASSERT( iAsyncFileWriter == NULL );
	iAsyncFileWriter = CAsyncFileWriter::NewL(*this,iFile, KMaxDstBufferSize );
	iAsyncFileWriter->CheckBufferOut( iDstDes );
#endif // defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)	
	}

void CImageEncoderPriv::SetFileL(RFile& aFile, const CImageEncoder::TOptions aOptions)
	{
	User::LeaveIfError(iFile.Duplicate(aFile));
	TInt pos = 0;
	User::LeaveIfError(iFile.Seek(ESeekStart, pos));
	User::LeaveIfError(iFile.Name(iFileName));
	iImageParameterData.SetFilenamePtr(&iFileName);
	iOptions = aOptions;
	
#if defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)	
	ASSERT( iAsyncFileWriter == NULL );
	iAsyncFileWriter = CAsyncFileWriter::NewL(*this, iFile, KMaxDstBufferSize );
	iAsyncFileWriter->CheckBufferOut( iDstDes );
#endif // defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)	
	}


void CImageEncoderPriv::SetThumbnail(TBool aDoGenerateThumbnail)
	{
	iPlugin->SetThumbnail(aDoGenerateThumbnail);
	}

void CImageEncoderPriv::SetDataL(HBufC8*& aDestinationData, const CImageEncoder::TOptions aOptions)
	{
	iDestination = &aDestinationData;
	iImageParameterData.SetDataPtr(aDestinationData);
	iOptions = aOptions;
	}

CImageEncoderPriv::CImageEncoderPriv(CImageEncodeConstruct* aConstruct, MImageEncoderPrivSupport* aSupport)
	: CActive(CActive::EPriorityIdle), iConstruct(aConstruct), iSupport(aSupport)
	{
	ASSERT(aConstruct);
	CActiveScheduler::Add(this);
	}

CImageEncoderPriv::~CImageEncoderPriv()
	{
	ASSERT(!IsActive());
	if (iPlugin)
		iPlugin->Cleanup();
	iFile.Close();
	delete iFinalDstBuffer;
	delete iPlugin;
#if defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)	
	delete iAsyncFileWriter;
#endif // defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)	
	}

void CImageEncoderPriv::CreatePluginL()
	{
	iPlugin = iConstruct->NewPluginL();
	iPlugin->iProperties = this;
	}

TInt CImageEncoderPriv::CurrentImageSizeL() const
	{
	TInt size;
	if( iImageParameterData.IsFilename())
		{
		ASSERT(iFile.SubSessionHandle() != 0);
		User::LeaveIfError(iFile.Size(size));
		}
	else
		{
		ASSERT(iFinalDstBuffer != NULL);
		size = iFinalDstBuffer->Length();
		}
	return size;
	}

void CImageEncoderPriv::Cleanup()
	{
	if(iImageParameterData.IsDataTypeDefined())
		{
		if (iImageParameterData.IsFilename() && (iFile.SubSessionHandle() != 0))
			{
			if(iFs != NULL)
				{
				// Delete file
				TFileName fileName;
				iFile.FullName(fileName);
				iFile.Close();
				iFs->Delete(fileName);
				}
			else
				{
				// Set file length zero
				iFile.SetSize(0);
				iFile.Close();
				}
			}
		}

	// Delete any objects we should get rid of
	delete iImageWriteCodec; iImageWriteCodec = NULL; // Created in Convert()
	delete iDstBuffer; iDstBuffer = NULL; // Created in Convert()
	
#if defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)	
	delete iAsyncFileWriter; iAsyncFileWriter = NULL;
#endif // defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)	
	}

void CImageEncoderPriv::InitConvertL()
	{
	ASSERT(iSource != NULL);
	iImageWriteCodec->InitFrameL(iDstDes, *iSource);
	}

void CImageEncoderPriv::RequestInitL()
	{
	__ASSERT_ALWAYS(!IsActive(), Panic(EConvertCalledWhileBusy));
	__ASSERT_ALWAYS(!iConvertStarted, Panic(EIllegalEncoderRestart));
				
	// once encoding starts, the encoder must not be reused
	iConvertStarted = ETrue;
	
#if defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)    
    if(!iImageParameterData.IsFilename())
#endif    
        {
        if (!iDstBuffer)
		    {
		    iDstBuffer = HBufC8::NewMaxL(KMaxDstBufferSize);
		    }        
        iDstDes.Set(iDstBuffer->Des());
        }

	iHeaderWritten=EFalse;
	}
	
void CImageEncoderPriv::Convert(RThread& aRequestThread, TRequestStatus* aRequestStatus, const CFbsBitmap& aSource, const CFrameImageData* aFrameImageData)
	{
	if (iConvertStarted)
		Panic(EIllegalEncoderRestart);
	
	if (IsActive())
		Panic(EConvertCalledWhileBusy);

	// once encoding starts, the encoder must not be reused
	iConvertStarted = ETrue;
	
	iRequestThread = &aRequestThread;
	iConvStatus = aRequestStatus;
	*iConvStatus = KRequestPending;
	iSource = &aSource;

	iFrameInfo.iOverallSizeInPixels = iSource->SizeInPixels();
	iFrameInfo.iFrameSizeInTwips = iSource->SizeInTwips();
	
	if(aSource.Handle()!=0 && aSource.ExtendedBitmapType()!=KNullUid)
        {
        RequestComplete(KErrNotSupported);
        return;
        }
	
	TRAPD(leaveErr, iPlugin->PrepareEncoderL(aFrameImageData));
	if (leaveErr != KErrNone)
		{
		RequestComplete(leaveErr);
		return;
		}
		
#if defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)		
    if (! iImageParameterData.IsFilename() )
#endif 
        {
        
    	if (!iDstBuffer)
    		{
    		TRAP(leaveErr, iDstBuffer = HBufC8::NewMaxL(KMaxDstBufferSize));
    		if (leaveErr != KErrNone)
    			{
    			RequestComplete(leaveErr);
    			return;
    			}
    		}

    	iDstDes.Set(iDstBuffer->Des());
        }
        
	TRAP(leaveErr, iPlugin->InitConvertL());
	if(leaveErr != KErrNone)
		{
		RequestComplete(leaveErr);
		return;
		}

	// we generate the exif data and write it
	// this method is asynchronous: it also starts generating the thumbnail
	TRequestStatus* status=&iStatus;
	SetSelfPending();
	TRAP(leaveErr, iPlugin->WriteExifDataL(status));
	if(leaveErr != KErrNone)
		{
		RequestComplete(leaveErr);
		return;
		}
	iHeaderWritten=EFalse;

	// the wating for the request to complete is initiated in the Image plugin
	}

void CImageEncoderPriv::RunL()
	{
	if(!iHeaderWritten)
		{
		if(iStatus.Int()!=KErrNone)
			{
			RequestComplete( iStatus.Int() );
			}
		else
			{
			// the thumbnail image has been created, we can now write it in the file
			TRAPD(leaveErr, iPlugin->WriteThumbnailL());
			if(leaveErr != KErrNone)
				{
				RequestComplete(leaveErr);
				return;
				}

			// Write the JFIF header
			TRAP(leaveErr,WriteDataPositionIncL(iStartPosition,iDstDes));
			if (leaveErr != KErrNone)
				{
				RequestComplete(leaveErr);
				return;
				}

			iHeaderWritten=ETrue;
			SelfComplete(KErrNone);
			}
		}
	else
		{
		iPlugin->DoConvert();
		}
	}

void CImageEncoderPriv::DoCancel()
	{
	iPlugin->Cleanup();
			
	// if SetSelfPending() has been called but not SelfComplete()
	// (i.e. the AO is active but there's still an outstanding request)
	// then we need to complete the request
	if (IsActive() && iStatus == KRequestPending)
		SelfComplete(KErrCancel);

	RequestComplete(KErrCancel);
	}

void CImageEncoderPriv::RequestComplete(TInt aReason)
	{
	ASSERT(iPlugin);
	iPlugin->NotifyComplete();
	ASSERT(iRequestThread);
	if (iConvStatus && (*iConvStatus == KRequestPending) )
		{
		iRequestThread->RequestComplete(iConvStatus, aReason);
		}
	}
	
void CImageEncoderPriv::WriteDataL(TInt aPosition,const TDesC8& aDes)
	{
	ASSERT(aPosition >= 0); 
	ASSERT(aDes.Length() >= 0);

	TInt  destSize = 0;
	
	if(iImageParameterData.IsFilename())
		{ // The source if a file
		ASSERT(iFile.SubSessionHandle() != 0); // The file has to be open
		User::LeaveIfError(iFile.Size(destSize));

		//If we start writing past EOF
		if (aPosition > destSize)
			{
			User::LeaveIfError(iFile.SetSize(aPosition));
			}
		User::LeaveIfError(iFile.Write(aPosition,aDes));
		// if the data has been appended to the file, we need to update the current position in the text
		}
	else
		{ // The source is a descriptor
		const TInt  totalSize = aPosition + aDes.Length();
		// totalSize is equal to the minimum length that the
		// destination descriptor has to be.  It is NOT the total
		// length of the destination descriptor - NJ

		if (iFinalDstBuffer == NULL)
			{
			iFinalDstBuffer = HBufC8::NewMaxL(totalSize);
			}
		destSize = iFinalDstBuffer->Length();

		//The buffer is too small
		if (destSize < totalSize)
			{
			iFinalDstBuffer = iFinalDstBuffer->ReAllocL(totalSize);
			}

		// check to see if we are extending the descriptor (eg. append)
		if (totalSize > destSize)
			{
			iFinalDstBuffer->Des().SetLength(totalSize);
			}

		// replace the current descriptor contents with the new data
		iFinalDstBuffer->Des().Replace(aPosition,aDes.Length(),aDes);
		}
	}

void CImageEncoderPriv::WriteDataPositionIncL(TInt aPosition,const TDesC8& aDes)
	{
	WriteDataL(aPosition, aDes);
	if(aPosition+aDes.Length() > iPosition)
		{
    	iPosition+=aDes.Length();
    	}
	}

void CImageEncoderPriv::SelfComplete(TInt aReason)
	{
	if (!IsActive())	// may already be active if SetSelfPending() called
		SetActive();
	TRequestStatus* status = &iStatus;
	User::RequestComplete(status, aReason);
	}

void CImageEncoderPriv::SetSelfPending()
	{
	iStatus = KRequestPending;
	SetActive();
	}

void CImageEncoderPriv::DoConvert()
	{
	TFrameState codecState = EFrameIncomplete;
	TRAPD(errCode, codecState = iImageWriteCodec->ProcessFrameL(iDstDes));

	HandleProcessFrameResult(errCode, codecState);
	}
	
#if defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)

/*virtual*/
void CImageEncoderPriv::BufferWritten(const TPtrC8& aWritten, TInt aErrorCode)
    {
    if (aErrorCode == KErrNone)
        {
        iAsyncFileWriter->CheckBufferIn(aWritten);
        }
    
    if (aErrorCode != KErrNone)
        {
        iWriterError = aErrorCode;
        RequestComplete( iWriterError );
        return;
        }
    }

void CImageEncoderPriv::HandleProcessFrameResult(TInt aErrCode, TFrameState aCodecState)	
    {
	if (aErrCode == KErrNone)
		{
		TRAP(aErrCode, DoHandleProcessFrameResultL(aCodecState) );
		}

	if (aErrCode != KErrNone)
	    {
	    RequestComplete(aErrCode);
	    }	
    }
    
void CImageEncoderPriv::DoHandleProcessFrameResultL(TFrameState aCodecState)
	{
	User::LeaveIfError( iWriterError );
	
    const TBool lastBuffer = (aCodecState == EFrameComplete || (aCodecState != EBlockComplete && iDstDes.Length() == 0));

	if(iImageParameterData.IsFilename())
		{ // The source if a file
		ASSERT(iFile.SubSessionHandle() != 0); // The file has to be open
		
		if (lastBuffer)
		    {
		    User::LeaveIfError( iAsyncFileWriter->FlushBuffers() );
		    }

		TPtr8 newBuffer(NULL, 0);
        iAsyncFileWriter->WriteBufferL(iDstDes, iStartPosition + iPosition);

        iAsyncFileWriter->CheckBufferOut( newBuffer );

    	if(iStartPosition + iPosition + iDstDes.Length() > iPosition)
    		{
        	iPosition+=iDstDes.Length();
        	}
        iDstDes.Set( newBuffer );
		}
	else
	    {
	    WriteDataPositionIncL(iStartPosition + iPosition, iDstDes);
	    }
	    
	if (lastBuffer || aCodecState == EBlockComplete)
		{
		if(lastBuffer)
			{
			FinishConvertL();
			}
		RequestComplete(KErrNone);
		}
	else	    
    	{
    	SelfComplete(KErrNone);
    	}
	}
	
#else

void CImageEncoderPriv::HandleProcessFrameResult(TInt aErrCode, TFrameState aCodecState)
	{
	if (aErrCode != KErrNone)
		{
		RequestComplete(aErrCode);
		return;
		}

	if (aCodecState == EFrameComplete || iDstDes.Length() == 0)
		iLastBuffer = ETrue;

	TRAPD(leaveErr,WriteDataPositionIncL(iStartPosition + iPosition, iDstDes));
	if (leaveErr != KErrNone)
		{
		RequestComplete(leaveErr);
		return;
		}

	if (iLastBuffer || aCodecState == EBlockComplete)
		{
		if(iLastBuffer)
			{
			TRAP(leaveErr, FinishConvertL());
			}
		iLastBuffer = EFalse;
		RequestComplete(leaveErr);
		}
	else
		{
		SelfComplete(KErrNone);
		}
	}
#endif // defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES)

TBufPtr8& CImageEncoderPriv::DestinationData()
	{
	return iDstDes;
	}

void CImageEncoderPriv::FinishConvertL()
	{
	iPlugin->UpdateHeaderL();
	if (!iImageParameterData.IsFilename())
		{
		*iDestination = iFinalDstBuffer;
		iFinalDstBuffer = NULL;
		}
	else
		{
		iFile.Close(); // close the file
		}
	}

TUid CImageEncoderPriv::ImplementationUid() const
	{
	return iConstruct->ImplementationUid();
	}

const CFrameImageData& CImageDecoderPriv::FrameData(TInt aFrameNumber) const
	{
	// Return the frame image data for a particular frame.
	__ASSERT_ALWAYS((aFrameNumber >= 0) && (aFrameNumber < iFrameInfo.Count()), Panic(EFrameNumberOutOfRange));
	return *iFrameData[aFrameNumber];
	}


/**
 * Passes on CustomSyncL to plugin
 *
 */

void CImageEncoderPriv::CustomSyncL(TInt aParam)
	{
#if ( defined(WRITER_EMULATE_SLOW_MEDIA) && defined(SYMBIAN_ENABLE_ENCODER_ASYNC_WRITES) )
// that's for testing/debugging only !
    if ( aParam == KUidIclTestEmulateSlowMedia.iUid && iAsyncFileWriter != NULL )
        {
        iAsyncFileWriter->iEmulateSlowMedia = ETrue;
        return;
        }
#endif 
	
	iPlugin->HandleCustomSyncL(aParam);
	}

/**
 * Plugin is interrogated with InitCustomAsyncL. If this works, then set internal Convert() cycle going
 *
 */

void CImageEncoderPriv::CustomAsync(RThread& aRequestThread, TRequestStatus* aRequestStatus, TInt aParam)
	{
	__ASSERT_ALWAYS(!IsActive(), Panic(EConvertCalledWhileBusy));
	
	TRAPD(error, iPlugin->InitCustomAsyncL(aParam));

	iRequestThread = &aRequestThread;
	iConvStatus = aRequestStatus;
	*iConvStatus = KRequestPending;

	if (error==KErrNone)
		SelfComplete(KErrNone);
	else
		RequestComplete(error);	
	}

/**
 *  Indicate if running in a separate thread
 * 
 */

TBool CImageEncoderPriv::AmInThread() const
	{
	return iSupport->AmInThread();
	}

/**
 *  Indicate if a decode should abort early (ie. following a Cancel). Always false unless in a thread.
 * 
 */

TBool CImageEncoderPriv::ShouldAbort() const
	{
	return iSupport->ShouldAbort();
	}
	
void CImageEncoderPriv::GetExtensionL(TUid aExtUid, MImageConvExtension*& aExtPtr)
	{
	iPlugin->GetExtensionL(aExtUid, aExtPtr);
	}
	
void CImageEncoderPriv::RegisterClientRequestStatus(RThread& aRequestThread, TRequestStatus* aRequestStatus)
	{
	__ASSERT_ALWAYS(!IsActive(), Panic(EConvertCalledWhileBusy));

	iRequestThread = &aRequestThread;
	iConvStatus = aRequestStatus;
	*iConvStatus = KRequestPending;
	}
	
void CImageEncoderPriv::StartActivity()
	{
	SetSelfPending();
	SelfComplete(KErrNone);
	}

// CImageDataArray
CImageDataArray::~CImageDataArray()
	{
	iImageData.ResetAndDestroy();
	iImageBuffers.ResetAndDestroy();
	}

// Inline these.
TInt CImageDataArray::InsertImageData(const TImageDataBlock* aEntry, TInt aPos)
	{
	ASSERT(aEntry != NULL);
	return iImageData.Insert(aEntry, aPos);
	}

TInt CImageDataArray::AppendImageData(const TImageDataBlock* aEntry)
	{
	ASSERT(aEntry != NULL);
	return iImageData.Append(aEntry);
	}

void CImageDataArray::RemoveImageData(TInt aIndex)
	{
	iImageData.Remove(aIndex);
	}

// Get image data/local image data.
const TImageDataBlock* CImageDataArray::GetImageData(TInt aIndex) const
	{
	return iImageData[aIndex];
	}

TImageDataBlock* CImageDataArray::GetImageData(TInt aIndex)
	{
	return iImageData[aIndex];
	}

// Get num entries.
TInt CImageDataArray::ImageDataCount() const
	{
	return iImageData.Count();
	}

// Append image buffer.
TInt CImageDataArray::AppendImageBuffer(const HBufC8* aImageBuffer)
	{
	return iImageBuffers.Append(aImageBuffer);
	}

