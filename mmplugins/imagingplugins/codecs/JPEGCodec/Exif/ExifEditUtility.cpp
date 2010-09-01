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

#include "exiftransform.h"
#include "exiftransformdataaccessor.h"
#include "ExifEditUtility.h"
#include "ExifThumbnailGenerator.h"
#include "ExifGeneralConsts.h"
#include "JpegConsts.h"
#include "ImageUtils.h"

#include <f32file.h>

const TUint8 KJpegMarkerByte = 0xff;
const TUint8 KDhtMarkerByte = 0xc4;
const TUint8 KDqtMarkerByte = 0xdb;
const TUint8 KSosMarkerByte = 0xda;


const TUint KTableTypeSize = 1;		// in DHT and DQT the table type is one byte long
const TUint KEntriesInQTable = 64;	// number of entries in a specific quantization table

const TUint KDCTableSize = 28;		// see Exif 2.2 specification
const TUint KACTableSize = 178;		// see Exif 2.2 specification

const TUint KDQTTypicalSize = (KTableTypeSize + KEntriesInQTable) * 3;
const TUint KDHTTypicalSize = (KTableTypeSize + KDCTableSize) * 2 + (KTableTypeSize + KACTableSize) * 2;

const TInt KJpegBlockTypeUndefined = -1;

_LIT(KDhtBlockHeader, "\xff\xc4\x00\x00");	// jpeg DHT block id followed by placeholder for block size
_LIT(KDqtBlockHeader, "\xff\xdb\x00\x00");	// jpeg DQT block id followed by placeholder for block size
	
CExifEditUtility* CExifEditUtility::NewL(MExifSource* aSource, MExifDest* aDest, TBool aIgnoreExifMetadataProcessing)
	{
	CExifEditUtility * self= new (ELeave) CExifEditUtility(aIgnoreExifMetadataProcessing);
	CleanupStack::PushL(self);
	self->ConstructL(aSource, aDest);
	CleanupStack::Pop(self);
	
	return self;
	}
	
CExifEditUtility::~CExifEditUtility()
	{
	Cancel();
	delete iScaledJpegGenerator;
	delete iExifData;
	iDHTTables.Close();
	iDQTTables.Close();	
	iTempCopyBuffer.Close();
	}
	
CExifEditUtility::CExifEditUtility(TBool aIgnoreExifMetadataProcessing)
	: CActive(EPriorityStandard)
	, iState(EEmpty)
	, iIgnoreExifMetadataProcessing(aIgnoreExifMetadataProcessing)
	{
	iLookupSlackBuff.SetLength( 0 );
	}
	
void CExifEditUtility::ConstructL(MExifSource* aSource, MExifDest* aDest)
	{
	ASSERT(aSource != NULL);
	ASSERT(aDest != NULL);
	iSource = aSource;
	iDest = aDest;
	CActiveScheduler::Add(this);
	}
	

MExifMetadata* CExifEditUtility::ExifMetadata()
	{
	return iExifData;
	}


//
//
//
//
//		interface methods
//	

void CExifEditUtility::ReadSourceL()
	{
	if(iState!=EEmpty)
		{
		User::Leave(KErrInUse);
		}
	
	InitReadL();
	if(!iIgnoreExifMetadataProcessing)
		{
		delete iExifData;
		iExifData = NULL;
		TRAPD(err, iExifData = iSource->ReadAndConvertExifDataL(iHeaderBlockSize + 
			(sizeof(KJpgSOISignature)+sizeof(KJpgMarker)) /*size of the app1 marker*/, 
			iExifBlockSize));
		if(err != KErrNone)
			{
			if(err == KErrNoMemory)
				{
				User::Leave(KErrNoMemory);					
				}
			iIgnoreExifMetadataProcessing = ETrue;
			iExifBlockSize = 0;
			iConvertToJfif = ETrue;				
			}

		}
	iState=EReadComplete;
	}
	

void CExifEditUtility::WriteDestL(TBool aEncodeThumbnail, const TSize& aSize, TBool aPreserveImage, TBool aMaintainAspectRatio, TRequestStatus* aNotifier)
	{
	iNotifier=aNotifier;
	iEncodeThumbnail=aEncodeThumbnail;
	iSize= aSize;
	iPreserveImage= aPreserveImage;
	iMaintainAspectRatio = aMaintainAspectRatio;
	if(iState !=EReadComplete)
		{
		RunError(KErrInUse);
		return;
		}
	
	TInt err=iDest->Init();
	if(err!=KErrNone)
		{
		RunError(err);
		return;
		}

	iState=EStartWrite;
	ProcessCommandL();
	*iNotifier=KRequestPending;
	}


//
//		interface methods
//	
//
//
//






//
//
//
//
//		methods from CActive
//
void CExifEditUtility::DoCancel()
	{
	iState=ECancelled;
		
	if(iScaledJpegGenerator)
		{
		iScaledJpegGenerator->Cancel();
		}
		
	//Now reset everything to how it was before the transform.
	iState = EReadComplete;	
	iSource->Cancel();
	iDest->Cancel();
			
	//We need to cancel both the internal state machine request status
	//and the client request status	
	if (iStatus == KRequestPending)
		{
		TRequestStatus* status=&iStatus;
		User::RequestComplete(status, KErrCancel);
		}
	if (iNotifier)//but we do need to check iNotifier TRequestStatus
		{
		if (*iNotifier == KRequestPending)
			{
			User::RequestComplete(iNotifier,KErrCancel);
			}
		}
	}


void CExifEditUtility::RunL()
	{
	if(iStatus.Int()==KErrNone)
		{
		ProcessCommandL();
		}
	else
		{
		RunError(iStatus.Int());
		}
	}

TInt CExifEditUtility::RunError(TInt aError)
	{					
	if(iState<EReadComplete)
		{
		delete iExifData;
		iExifData = NULL;
		iState=EEmpty;
		iSource->CleanupAfterEarlyError();
		iDest->CleanupAfterEarlyError();
		}
	else 
		{
		iState=EReadComplete;
		iSource->CleanupAfterLateError();
		iDest->CleanupAfterLateError();
		}
		
	User::RequestComplete(iNotifier, aError);
	return KErrNone;
	}
//
//		interface methods
//	
//
//
//


void CExifEditUtility::ProcessCommandL()
	{
	TBool doContinue=EFalse;
	switch(iState)
		{
		// writer commands
		case EStartWrite:
			doContinue=DoCopyBlockL(0, iHeaderBlockSize, EWriteWriteHeader);
			iState=ECopying;
			break;
		case EWriteWriteHeader:
			if(!iIgnoreExifMetadataProcessing)
				{
				doContinue=DoWriteReadThumbnailL();
				}
			else
				{
				//skip to next step.
				iStatus=KRequestPending;
				TRequestStatus* reqStat=&iStatus;
				User::RequestComplete(reqStat, KErrNone);
				doContinue = ETrue; 
				}
			iState = EReadThumbnail;
			break;
		case EReadThumbnail:
			doContinue=DoReadThumbnailL();		
			if (iPreserveImage)
				{
				iState=EWriteReadThumbnail;
				}
			else
				{
				iState=EWriteWriteExif;
				}
			break;		
		case EWriteReadThumbnail:
			doContinue=DoWriteConvertExifL();
			iState=EWriteConvertExif;
			break;
		case EWriteConvertExif:
			if(iIgnoreExifMetadataProcessing && (iExifBlockSize >0))
				{
				HBufC8* destBuffer = iIOBufferPtr.AllocL();
				iDest->SetDestBuffer(destBuffer); // Give iDest ownership of buffer
				}
			doContinue=DoWriteDestBufferL();
			
			if(iPreserveImage)
				{
				iState=EWriteWriteExif;
				}
			else
				{
				iState=EWriteReadMainImage;
				}
			break;
		case EWriteWriteExif:
			if(iPreserveImage)
				{
				doContinue=DoCopyBlockL(iTrailerOffset, iTrailerBlockSize, EWriteTrailer, iDestIsExif);
				iState=ECopying;
				}
			else
				{
				doContinue=DoCreateMainImageL();
				iCurrentWriteBufferIndex = 0;
				iState=EWriteReadThumbnail;
				}
			break;
		case EWriteTrailer:
			DoWriteComplete();
			iState=EReadComplete;
			break;
		case EWriteReadMainImage:
			doContinue=DoWriteScaledImageL();
			break;
		case ECancelled:
			return;
		case ECopying:
			iState=DoCopyNextBlockL();
			doContinue=ETrue;
			break;
		default:
			break;
		}
		
	if(doContinue)
		{
		SetActive();
		}
	}




//
//
//
//
//		Readers and writers
//	each performs one step of the process


//
//			Readers
//
void CExifEditUtility::InitReadL()
	{
	iSource->InitL();
	
	TBool exifFound=EFalse;
	
	TUint16 jpegMarker=0;
	TUint position=0;
	TUint16 blockSize=0;
	// we want to preserve the image data.
	// so we parse the source file, and isolate the exif metadata
	for(;;)
		{		
		position+=blockSize;
		jpegMarker=KJpgSOISignature;
		User::LeaveIfError(iSource->SetReadPosition(position));
		do
			{
			jpegMarker=iSource->ReadUint16L();	
			position+=sizeof(TUint16);
			}while(jpegMarker==KJpgSOISignature);

		if(jpegMarker==KJpgApp0Signature || jpegMarker==KJpgApp1Signature)
			{
			blockSize=iSource->ReadUint16L();
			position+=sizeof(TUint16);
			if (jpegMarker==KJpgApp1Signature)
				{
				exifFound=ETrue;
				break;					
				}
			}
		else
			{
			break;
			}
		}

	User::LeaveIfError(iSource->Size(iTrailerBlockSize));
	iTrailerOffset=sizeof(KJpgSOISignature);
	if(exifFound)
		{
		// Set our reading parameters
		TUint sizeOfApp1Marker = sizeof(KJpgSOISignature)+sizeof(KJpgMarker);
		if(position <= sizeOfApp1Marker) User::Leave(KErrCorrupt);//source file is corrupted.
		iHeaderBlockSize=position - sizeOfApp1Marker;	
		iExifBlockSize=blockSize;
		iTrailerOffset+=iHeaderBlockSize+blockSize;
		}
	else
		{
		iHeaderBlockSize=sizeof(KJpgSOISignature); 		
		iExifBlockSize=0;
		}
	iTrailerBlockSize-= iTrailerOffset;
	}

//			Readers
//
//


//
//			Writers
//
TBool CExifEditUtility::DoWriteConvertExifL()
	{
	if(iIgnoreExifMetadataProcessing)
		{
		if(iExifBlockSize)
			{
			//copy the EXIF metadata en-block, without parsing it first.
			iSource->ReadL(iHeaderBlockSize, iIOBufferPtr, iExifBlockSize + KBlockSizeLength, iStatus);
			iDestIsExif = ETrue;
			}
		else
			{ // JUMP to next step... no EXIF metadata information, so nothing to ignore.
			if (iConvertToJfif)
				{
				//Create a buffer to hold a default JFIF header
				HBufC8* buffer=HBufC8::NewMaxL(KJfifApp0DataSize+sizeof(KJpgApp0Signature));
				TPtr8 buf(buffer->Des());
				TJpegUtilities::CreateJfifHeader(buf);
				iDest->SetDestBuffer(buffer);// Give iDest ownership of buffer				
				}

			iStatus=KRequestPending;	
			TRequestStatus* reqStat=&iStatus;
			User::RequestComplete(reqStat, KErrNone);
			iDestIsExif = EFalse;
			}
		}
	else
		{	
		TInt err = KErrNone;
		if(iExifBlockSize > 0 || iExifData->IsExifDataModified())
			{
			HBufC8* buffer = NULL;
			
			UpdateImageSizeTagsL();
			iExifData->CheckUpdateMandatoryTagsL();
			
			err= iExifData->CreateExifChunk(buffer); // buffer not owned by iExifData
			if (err == KErrNone)
				{
				iDest->SetDestBuffer(buffer); // Give iDest ownership of buffer
				}	
			iExifData->ResetExifDataModified();
			iDestIsExif = ETrue;	
			}
		else
			{
			iDestIsExif = EFalse;
			}
		iStatus=KRequestPending;	
		TRequestStatus* reqStat=&iStatus;
		User::RequestComplete(reqStat, err);	
		}
	return ETrue;	//we continue to the next step
	}

TBool CExifEditUtility::DoWriteReadThumbnailL()
	{
	if(iEncodeThumbnail)
		{
		HBufC8* buffer = iExifData->GetJpegThumbnailData(); // Does not transfer ownership of buffer
		if(buffer==NULL)
			{
			delete iScaledJpegGenerator;
			iScaledJpegGenerator=NULL;
			// we have to generate the thumbnail
			iScaledJpegGenerator=CScaledJpegGenerator::NewL(&iStatus, iSource);
			iScaledJpegGenerator->StartL(TSize(KThumbnailWidth, KThumbnailHeight), CScaledJpegGenerator::EThumbnail);
			}
		else
			{
			iStatus=KRequestPending;
			TRequestStatus* reqStat=&iStatus;
			User::RequestComplete(reqStat, KErrNone);
			}
		}
	else
		{
		iExifData->SetThumbnailData(NULL);
		iStatus=KRequestPending;
		TRequestStatus* reqStat=&iStatus;
		User::RequestComplete(reqStat, KErrNone);
		}

	return ETrue;
	}

TBool CExifEditUtility::DoReadThumbnailL()
	{
	
	if(iEncodeThumbnail && iScaledJpegGenerator)
		{
		HBufC8* thumb=iScaledJpegGenerator->GetJpegDataL(); // thumb not owned by iScaledJpegGenerator
		delete iScaledJpegGenerator;
		iScaledJpegGenerator=NULL;
		iExifData->SetThumbnailData(thumb); // Give iExifData ownership of thumb
		}
	iStatus=KRequestPending;	
	TRequestStatus* reqStat=&iStatus;
	User::RequestComplete(reqStat, KErrNone);
	return ETrue;
	}

TBool CExifEditUtility::DoCreateMainImageL()
	{
	if(!iScaledJpegGenerator)
		{
		iScaledJpegGenerator=CScaledJpegGenerator::NewL(&iStatus, iSource, iMaintainAspectRatio, 
			CImageDecoder::EOptionIgnoreExifMetaData);
		}
	iScaledJpegGenerator->StartL(iSize, CScaledJpegGenerator::EMainImage);
	return ETrue;
	}

TBool CExifEditUtility::DoWriteScaledImageL()
	{
	HBufC8* buffer = iScaledJpegGenerator->JpegDataBufferL(iCurrentWriteBufferIndex);
	if (buffer)
		{
		iCurrentWriteBufferIndex++;
		iState = EWriteReadMainImage;
		}
	else
		{
		iState = EWriteTrailer;
		}
	iDest->SetDestBuffer(buffer); // Give iDest ownership of buffer
	return DoWriteDestBufferL();
	}

void CExifEditUtility::SetUpTablesL()
	{
	// set up the descriptors that store the combined tables

	iDHTTables.Zero();
	if (iDHTTables.MaxLength() < KDHTTypicalSize)
		{
		iDHTTables.ReAllocL(KDHTTypicalSize);
		}
	iDHTTables.Append(KDhtBlockHeader);	// write the DHT block id and placeholder size
	
	iDQTTables.Zero();
	if (iDQTTables.MaxLength() < KDQTTypicalSize)
		{
		iDQTTables.ReAllocL(KDQTTypicalSize);
		}
	iDQTTables.Append(KDqtBlockHeader);	// write the DQT block id and placeholder size
	}

void CExifEditUtility::AppendTableDataL(RBuf8& aTables, const TDesC8& aData)
	{
	if (aTables.Length() + aData.Length() > aTables.MaxLength())
		{
		aTables.ReAllocL(aTables.Length() + aData.Length());
		}
	aTables.Append(aData);
	}
	
TBool CExifEditUtility::DoCopyBlockL(TUint aStart, TInt aLength, TConvertState aNextStep, TBool aCombineTables)
	{
	iCurrentJpegBlockType = KJpegBlockTypeUndefined;
	iCurrentJpegBlockSize = 0;

	TUint bytesToRead = 0;
	if (aCombineTables)
		{
		// EXIF only supports one DHT table and one DQT table (JFIF supports multiple tables)
		// When copying the data, keep track of jpeg blocks and store any DHT and DQT tables
		// Write the combined tables out before the SOS block

		SetUpTablesL();

		iCurCopyState = EReadJpegBlockInfo;
		bytesToRead = KBlockIdAndSizeLength;
		}
	else
		{
		// copy the data in multiples of KIOBlockSize
		iCurCopyState = ERead;				
		bytesToRead = Min(aLength, KIOBlockSize);
		}

	iSource->ReadL(aStart, iIOBufferPtr, bytesToRead, iStatus);
	iNextStep = aNextStep;
	iCopyLength = aLength - bytesToRead;
	return ETrue;	//we continue to the next step
	}

void CExifEditUtility::DoReadL()
	{
	TUint bytesToRead = 0;
	if (iCurrentJpegBlockType == KJpegBlockTypeUndefined)
		{
		// start/continue reading data without tracking jpeg block info
		iCurCopyState = ERead;
		bytesToRead = Min(iCopyLength, KIOBlockSize);
		}
	else if (iCurrentJpegBlockSize == 0)
		{
		// read the next jpeg block's info
		iCurCopyState = EReadJpegBlockInfo;
		bytesToRead = KBlockIdAndSizeLength - iLookupSlackBuff.Length();
		}
	else
		{
		// start/continue reading data from this jpeg block
		iCurCopyState = ERead;
		bytesToRead = Min(iCurrentJpegBlockSize, KIOBlockSize);
		iCurrentJpegBlockSize -= bytesToRead;	// keep track of how much block data is left to read 
		}
	iSource->NextReadL(iIOBufferPtr, bytesToRead, iStatus);
	iCopyLength -= bytesToRead;	
	}

void CExifEditUtility::HandleJpegBlockL(const TDesC8& aPrependedData)
	{
	switch (iCurrentJpegBlockType)
		{
	case KDhtMarkerByte:
	case KDqtMarkerByte:
		DoReadL();
		break;
	case KSosMarkerByte:
		{
		// reached the SOS block so write out the tables
		
		// calculate the size of the combined DHT table data
		TInt totalSize = iDHTTables.Length() - KBlockIdLength;
		
		// fill in the size placeholder
		iDHTTables[2] = totalSize >> 8;
		iDHTTables[3] = totalSize & 0xff;
		
		AppendTableDataL(iDHTTables, iIOBufferPtr);

		// calculate the size of the combined DQT table data
		totalSize = iDQTTables.Length() - KBlockIdLength;

		// fill in the size placeholder
		iDQTTables[2] = totalSize >> 8;
		iDQTTables[3] = totalSize & 0xff;
		
		AppendTableDataL(iDQTTables, iDHTTables);

		iDest->WriteL(iDQTTables, iDQTTables.Length(), iStatus);
		break;
		}
    case (KJpgCommentSignature & 0xFF):
	case (KJpgApp0Signature & 0xFF):
        if (iDestIsExif)
            {
            // According to the EXIF 2.2 spec. we can't have comments
            // and shouldn't have APP0, so skip them.
			// APP0 can be skipped entirely using the block length.
			// In case of comment block, crawl slowly, as the comment blocks may be corrupted
			if(iCurrentJpegBlockType == (KJpgApp0Signature & 0xFF))
            	{
	            TInt fileSize = 0;
				User::LeaveIfError(iSource->Size(fileSize));
				ASSERT(fileSize > 0);
	            TUint positionAfterCurBlock = fileSize - iCopyLength + iCurrentJpegBlockSize;
	            if(positionAfterCurBlock > fileSize)
	            	{
	            	User::Leave(KErrCorrupt);
	            	}
				User::LeaveIfError(iSource->SetReadPosition(positionAfterCurBlock));            	
            	}
            iCurrentJpegBlockType = 0;
            iCurrentJpegBlockSize = 0;
            DoReadL();
            break;	
            }	
            // if we'are not in EXIF mode fall to default block handling
	default:
	    {
 	    TInt len = aPrependedData.Length() +  iIOBufferPtr.Length();
 	    if (iTempCopyBuffer.MaxLength() < len)
 	    	{
 	    	iTempCopyBuffer.ReAllocL(len);
 	    	}
 	    iTempCopyBuffer = aPrependedData;
        iTempCopyBuffer.Append( iIOBufferPtr );
        iDest->WriteL(iTempCopyBuffer, len, iStatus);
		break;
		}
	    } // switch
	}

TConvertState CExifEditUtility::DoCopyNextBlockL()
	{
	TConvertState result = ECopying;
	
	switch (iCurCopyState)
		{
	case EReadJpegBlockInfo:
		{
		iCurCopyState = EWrite;

		// iIOBuffer should contain in order:
		// - the jpeg marker byte (0xff)
		// - the block id byte
		// - the high byte of the block size
		// - the low byte of the block size
		const TText8* dataPtr = iIOBufferPtr.Ptr();
		const TText8* const dataPtrLimit = dataPtr + iIOBufferPtr.Length();
		if (iLookupSlackBuff.Length()==0 ) 
		    {
    		while (dataPtr < dataPtrLimit && *dataPtr != KJpegMarkerByte)
    		    {
    		    dataPtr++;
    		    }
    		const TInt dataLeft = TUint(dataPtrLimit - dataPtr);
    		ASSERT(dataLeft >= 0 );
    		if (dataLeft < KBlockIdAndSizeLength) 
    		    {
    		    if (dataLeft > 0) // some partial marker, take a copy of that data
    		        {
    		        iLookupSlackBuff.Copy(dataPtr, dataLeft);
    		        }
    		    DoReadL();   
    		    break;
    		    }

    		// we should have some marker and block length in the buffer
		    iCurrentJpegBlockType = dataPtr[1];
		    iCurrentJpegBlockSize = (dataPtr[2] << 8) + dataPtr[3] - KBlockSizeLength;    		    
		    }
		else    // we have some marker in a slack buffer, so combine it with current data
		    {
		    if (iLookupSlackBuff.Length() + iIOBufferPtr.Length() < KBlockIdAndSizeLength)
		        {
		        User::Leave(KErrCorrupt); // unexpected and of image data
		        }
		    TInt needToAppend = KBlockIdAndSizeLength - iLookupSlackBuff.Length();
		    iLookupSlackBuff.Append( iIOBufferPtr.Left( needToAppend ) );
		    iIOBufferPtr.Shift( needToAppend );
		    iCurrentJpegBlockType = iLookupSlackBuff[1];
		    iCurrentJpegBlockSize = (iLookupSlackBuff[2] << 8) + iLookupSlackBuff[3] - KBlockSizeLength;    		    		    
		    }
		//check validity of the block size
		if(iCurrentJpegBlockSize > iCopyLength)        
			{        
			User::Leave(KErrCorrupt);        
			}        

		HandleJpegBlockL(iLookupSlackBuff);
        iLookupSlackBuff.SetLength( 0 );
		break;
		}
	case ERead:
		{
		iCurCopyState = EWrite;

		switch (iCurrentJpegBlockType)
			{
		case KDhtMarkerByte:
			// store the DHT block data
			AppendTableDataL(iDHTTables, iIOBufferPtr);
			DoReadL();
			break;
		case KDqtMarkerByte:
			// store the DQT block data
			AppendTableDataL(iDQTTables, iIOBufferPtr);
			DoReadL();
			break;
		default:
			iDest->WriteL(iIOBufferPtr, iIOBufferPtr.Length(), iStatus);
			ASSERT(iCopyLength >= 0);
			if (iCopyLength == 0)
				{
				result = iNextStep;
				}
			break;
			}
		
		break;
		}
	case EWrite:
		{
		if (iCurrentJpegBlockType == KSosMarkerByte)
			{
			iCurrentJpegBlockType = KJpegBlockTypeUndefined;	// tables have been written out, no need to track the jpeg blocks now
			}
		DoReadL();
		break;
		}
	default:
		break;
		}

	return result;
	}


TBool CExifEditUtility::DoWriteComplete()
	{
	iDest->WriteComplete();
	
	User::RequestComplete(iNotifier, KErrNone);
	return EFalse;	
	}


TBool CExifEditUtility::DoWriteDestBufferL()
	{
	iDest->WriteDestBufferL(iStatus);
	return ETrue;
	}
//			Writers
//
//

//
//		Readers and writers
//	each performs one step of the process
//
//
//

	
void CExifEditUtility::UpdateImageSizeTagsL()
	{
	if (iPreserveImage)
		{
		if (!iExifData->CheckImageSizeTags())
			{
			TSize destSize;
			// In this case, jpeg data blocks are copied from source to dest, 
			// so we need CImageDecoder to get the size of the image.
			// NOTE: normally image size would be copied from valid Exif source 
			// but this is for the case where that data is missing from the source.
			CImageDecoder* decoder = NULL;
			TRAPD(err, decoder = iSource->CreateImageDecoderL(CImageDecoder::EOptionNone));
			if (err != KErrNone)
				{
				destSize = iSize;
				}
			else
				{
				destSize = decoder->FrameInfo().iFrameCoordsInPixels.Size();
				delete decoder;
				}
			iExifData->UpdateImageSizeTagsL(destSize);
			}
		}
	else
		{
		TSize destSize;
		if (iScaledJpegGenerator) 
			{
			iScaledJpegGenerator->GetScaledImageSize(destSize);
			}
		else
			{
			destSize = iSize;
			}
		iExifData->UpdateImageSizeTagsL(destSize);
		}
	}

	
