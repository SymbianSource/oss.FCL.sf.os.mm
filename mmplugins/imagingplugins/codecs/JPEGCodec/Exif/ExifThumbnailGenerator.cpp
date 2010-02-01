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

#include "ExifThumbnailGenerator.h"
#include <imageconversion.h>
#include <bitmaptransforms.h>
#include "JpegTypes.h"
#include "jpegwritecodec.h"

#include "ExifGeneralConsts.h"

#include "exiftransformdataaccessor.h"

CScaledJpegGenerator* CScaledJpegGenerator ::NewL(TRequestStatus* aNotifier, const MExifSource* aSource, 
	TBool aMaintainAspectRatio, CImageDecoder::TOptions aDecoderOptions)
	{
	CScaledJpegGenerator* self= new (ELeave) CScaledJpegGenerator(aNotifier, aMaintainAspectRatio, aDecoderOptions);
	CleanupStack::PushL(self);
	self->ConstructL(aSource);
	CleanupStack::Pop(self);
	
	return self;	
	}
	
HBufC8* CScaledJpegGenerator::GetJpegDataL()
	{
	if(iState!=EFinished)
		{
		User::Leave(KErrInUse);
		}
	if(iJPegImage.Count()==0)
		{
		User::Leave(KErrNotFound);
		}

	TInt size=0;
	TInt i;
	for(i=0; i<iJPegImage.Count(); i++)
		{
		size += iJPegImage[i]->Length();
		}
	HBufC8* thumbnailData=HBufC8::NewL(size);
	TPtr8 bufPtr(thumbnailData->Des());
	
	for(i=0; i<iJPegImage.Count(); i++)
		{
		bufPtr.Append(*(iJPegImage[i]));
		}
	
	return thumbnailData;
	}

HBufC8* CScaledJpegGenerator::JpegDataBufferL(const TUint aIndex)
	{
	if (iState != EFinished)
		{
		User::Leave(KErrInUse);
		}
	if (iJPegImage.Count() == 0)
		{
		User::Leave(KErrNotFound);
		}

	if (aIndex < iJPegImage.Count())
		{
		return iJPegImage[aIndex]->AllocL();
		}
	else
		{
		return NULL;
		}
	}
	
CScaledJpegGenerator::CScaledJpegGenerator(TRequestStatus* aNotifier, TBool aMaintainAspectRatio, 
	CImageDecoder::TOptions aDecoderOptions)
	: CActive(EPriorityStandard), iNotifier(aNotifier), iState(EFinished), 
	iMaintainAspectRatio(aMaintainAspectRatio)
	{	
	iDecoderOptions = aDecoderOptions;
	}

CScaledJpegGenerator::~CScaledJpegGenerator()
	{
	Cancel();
	CleanData();
	iJPegImage.Close();
	}
	
void CScaledJpegGenerator::ConstructL(const MExifSource* aSource)
	{
	ASSERT(aSource != NULL);
	iSource = aSource;
	CActiveScheduler::Add(this);
	}
	


void CScaledJpegGenerator::StartL(TSize aSize, TImageToGenerate aImageType)
	{
	if(iState!=EFinished)
		{
		User::Leave(KErrInUse);
		}
	
	
	CleanResultData();
	CleanTempData();
	iSize=aSize;
	iImageType=aImageType;
	
	if(!iImageDecoder)
		{
		iImageDecoder = iSource->CreateImageDecoderL(iDecoderOptions);
		TFrameInfo frameInfo= iImageDecoder->FrameInfo();

		if(iBitmap)
			{
			delete iBitmap;
			iBitmap = NULL;
			}
		iBitmap= new (ELeave) CFbsBitmap();
		
		User::LeaveIfError(iBitmap->Create(frameInfo.iFrameCoordsInPixels.Size(), frameInfo.iFrameDisplayMode));
		iState=EDecode;
		iImageDecoder->Convert(&iStatus, *iBitmap);
		SetActive();
		}
	else
		{	// the initialisation has already been done, there is no need to do it again
		DoScaleBitmapL();
		SetActive();
		}
	*iNotifier=KRequestPending;
	}

void CScaledJpegGenerator::RunL()
	{
	if(iStatus.Int()==KErrNone)
		{
		switch(iState)
			{
			case EDecode:
				// the decode is finished, we now scale the image
				DoScaleBitmapL();
				SetActive();
				break;
				
			case EScale:
				 // the scaling is over. We now Convert the image
				DoConvertL();
				SetActive();
				break;
			case EConvert:
				{
				switch(iFrameState)
					{
					case EFrameIncomplete:
						DoConvertL();
						SetActive();
						break;
					case EFrameComplete:
						GenerationCompleteL();
						break;
					default:
						iStatus=KErrCorrupt;
						RunError(KErrCorrupt);
					}
				}
				break;
			case EFinished:
				break;
			default:
				break;
			}
		// we have the bitmap, now scale it
		}
	else
		{
		RunError(iStatus.Int());
		}
	}
	
void CScaledJpegGenerator::DoCancel()
	{
	if (iImageDecoder)
		{
		iImageDecoder->Cancel();
		}
	if (iBitmapScaler)
		{
		iBitmapScaler->Cancel();
		}
	CleanData();
	User::RequestComplete(iNotifier, KErrCancel);
	iState=EFinished;
	}
	
void CScaledJpegGenerator::DoScaleBitmapL()
	{
	if(iBitmapScaler)
		{
		delete iBitmapScaler;
		iBitmapScaler = NULL;
		}
	iBitmapScaler=CBitmapScaler::NewL();

	iBitmapScaler->UseLowMemoryAlgorithm(ETrue);
	iBitmapScaler->Scale(&iStatus, *iBitmap, iSize, iMaintainAspectRatio);
	iState=EScale;
	}


 // returns ETrue if it is the last chunk of data to be received
void CScaledJpegGenerator::DoConvertL()
	{
	if(iState!=EConvert)
		{
		delete iBitmapScaler;
		iBitmapScaler=NULL;
		CreateJpegCodecL();
		if(iJpegChunck)
			{
			delete iJpegChunck;
			iJpegChunck = NULL;
			}
		iJpegChunck=HBufC8::NewMaxL(4096);
		iJpegChunckPtr.Set(*iJpegChunck);

		if(iImageType==EThumbnail)
			{
			// write SOI
			iJpegChunckPtr[0] = (TUint8)((KJpgSOISignature & 0xff00) >> 8);
			iJpegChunckPtr[1] = (TUint8)(KJpgSOISignature & 0xff);
			iJpegChunckPtr.Shift(2);
			}
		
		iJpegCodec->InitFrameL(iJpegChunckPtr, *iBitmap);
		
		if(iImageType==EThumbnail)
			{
			// Adjust iJpegChunckPtr to account for SOI added above 
			TInt len=iJpegChunckPtr.Length() + 2;
			iJpegChunckPtr.Set(*iJpegChunck);
			iJpegChunckPtr.SetLength(len);		
			}
			
		HBufC8* buf = iJpegChunckPtr.AllocLC();
		iJPegImage.AppendL(buf);
		CleanupStack::Pop(buf);
		iJpegChunckPtr.Set(*iJpegChunck);
		iFrameState=EFrameIncomplete;
		}
	else
		{
		iFrameState= iJpegCodec->ProcessFrameL(iJpegChunckPtr);
		HBufC8* buf = iJpegChunckPtr.AllocLC();
		iJPegImage.AppendL(buf);
		CleanupStack::Pop(buf);
		}	
	

	iStatus=KRequestPending;
	TRequestStatus* status=&iStatus;
	User::RequestComplete(status, KErrNone);
	iState=EConvert;
	}


void CScaledJpegGenerator::CreateJpegCodecL()
	{
	const CFrameImageData* frameImageData= &iImageDecoder->FrameData();

	//PDEF115929
	//This should not be the case - corrupt file
	if (frameImageData == NULL)
		{
		User::Leave(KErrCorrupt);
		}

	TJpgFrameInfo frameInfo;
	TInt qualityFactor;

	qualityFactor = 0;

 	TInt count = frameImageData->ImageDataCount();

	//PDEF115929
	//This should not be the case - corrupt file
	if (count == 0)
		{
		User::Leave(KErrCorrupt);
		}

	TBool jpgImageDataProcessed = EFalse;
	for (TInt index = 0 ; index<count ; index++)
		{	
		const TImageDataBlock& encoderData = *frameImageData->GetImageData(index);
		if (encoderData.DataType() == KJPGImageDataUid)
			{
			// Leave if we have already processed one of these.
			if (jpgImageDataProcessed)
				User::Leave(KErrCorrupt);

			const TJpegImageData& jpegImageData = STATIC_CAST(const TJpegImageData&, encoderData);
			qualityFactor = jpegImageData.iQualityFactor;

			TJpegImageData::TColorSampling sampleScheme = jpegImageData.iSampleScheme;
			frameInfo.iComponent[0].iQTable = 0;
			if (sampleScheme == TJpegImageData::EMonochrome)
				{
				frameInfo.iNumberOfComponents = 1;
				frameInfo.iComponent[0].iHorzSampleFactor = 1;
				frameInfo.iComponent[0].iVertSampleFactor = 1;
				}
			else
				{
				frameInfo.iNumberOfComponents = 3;
				switch (sampleScheme)
					{
				case TJpegImageData::EColor420:
					frameInfo.iComponent[0].iHorzSampleFactor = 2;
					frameInfo.iComponent[0].iVertSampleFactor = 2;
					frameInfo.iComponent[1].iVertSampleFactor = 1;
					frameInfo.iComponent[2].iVertSampleFactor = 1;
					break;
				case TJpegImageData::EColor422:
					frameInfo.iComponent[0].iHorzSampleFactor = 2;
					frameInfo.iComponent[0].iVertSampleFactor = 1;
					frameInfo.iComponent[1].iVertSampleFactor = 1;
					frameInfo.iComponent[2].iVertSampleFactor = 1;
					break;
				case TJpegImageData::EColor444:
					frameInfo.iComponent[0].iHorzSampleFactor = 1;
					frameInfo.iComponent[0].iVertSampleFactor = 1;
					frameInfo.iComponent[1].iVertSampleFactor = 1;
					frameInfo.iComponent[2].iVertSampleFactor = 1;
					break;
				default:
					User::Leave(KErrNotSupported);
					}

				frameInfo.iComponent[1].iHorzSampleFactor = 1;
				frameInfo.iComponent[1].iQTable = 1;
				frameInfo.iComponent[2].iHorzSampleFactor = 1;
				frameInfo.iComponent[2].iQTable = 1;
				}
			
			frameInfo.iMaxHorzSampleFactor = frameInfo.iComponent[0].iHorzSampleFactor;
			frameInfo.iMaxVertSampleFactor = frameInfo.iComponent[0].iVertSampleFactor;

			jpgImageDataProcessed = ETrue;
			}
		else if (encoderData.DataType() == KJPGQTableUid)
			{
			const TJpegQTable& jpegQTable = STATIC_CAST(const TJpegQTable&, encoderData);
			TInt tableIndex = jpegQTable.iTableIndex;
			if ((tableIndex != TJpegQTable::ELumaTable) && (tableIndex != TJpegQTable::EChromaTable))
				{
				// This can happen if client has given us a second chroma QTable
				// replacement.  We do not support this at present, so we ignore
				// the extra Q table and encode using the same QTable for both
				// chroma components.
				continue;
				}

			TUint8 values[KJpgQTableEntries];
			TUint8* valuePtr = values;
			const TUint8* zigZagPtr = KZigZagSequence.iZigZag;
			const TUint8* valuePtrLimit = valuePtr + KJpgQTableEntries;

			while (valuePtr < valuePtrLimit)
				*valuePtr++ = jpegQTable.iEntries[*zigZagPtr++];

			if (tableIndex == TJpegQTable::ELumaTable)
				{
				delete iLumaTable;
				iLumaTable=NULL;
				iLumaTable = new(ELeave) TQTable;

				iLumaTable->Set(values, EFalse);
				}
			else // tableIndex must be TJpegQTable::EChromaTable
				{
				delete iChromaTable;
				iChromaTable=NULL;
				iChromaTable = new(ELeave) TQTable;

				iChromaTable->Set(values, EFalse);
				}
			}
		else if ((encoderData.DataType() == KJPGCommentUid))
			{
			if (iImageType == EMainImage) 
				{
				const TJpegComment& jpegComment = STATIC_CAST(const TJpegComment&, encoderData);
				if (!jpegComment.iComment)
					User::Leave(KErrNotFound);

				if ((jpegComment.iComment->Length() == 0) || (jpegComment.iComment->Length() > 65534))
					User::Leave(KErrNotSupported);
	            
	        	HBufC8* thisComment = jpegComment.iComment->AllocL();
		    	TInt ret = iComment.Append(thisComment);
			    if (ret != KErrNone)
			    	{
			    	delete thisComment;
			    	User::Leave(ret);
				    }
				
				}
			}
		else
			User::Leave(KErrCorrupt);
		}

	iJpegCodec= CJpgWriteCodec::NewL(frameInfo,qualityFactor,iLumaTable,iChromaTable,iComment,NULL);
	}


TInt CScaledJpegGenerator::RunError(TInt aError)
	{
	CleanData();
	User::RequestComplete(iNotifier, aError);
	iState=EFinished;
	
	return KErrNone;
	}
	
void CScaledJpegGenerator::GenerationCompleteL()
	{
	// write EOI
	TBuf8<2> buffer2Bytes;
	buffer2Bytes.SetLength(2);
	TUint8* headerPtr2 = &buffer2Bytes[0];
	headerPtr2[0] = (TUint8)((KJpgEOISignature & 0xff00) >> 8);
	headerPtr2[1] = (TUint8)(KJpgEOISignature & 0xff);
	HBufC8* buf = buffer2Bytes.AllocLC();
	iJPegImage.AppendL(buf);
	CleanupStack::Pop(buf);
	iState=EFinished;
	CleanTempData();
	User::RequestComplete(iNotifier, KErrNone);
	}

void CScaledJpegGenerator::CleanTempData()
	{
	delete iJpegCodec;
	iJpegCodec=NULL;
	delete iBitmapScaler;
	iBitmapScaler=NULL;

	delete iJpegChunck;
	iJpegChunck=NULL;

	iComment.ResetAndDestroy();
	delete iLumaTable;
	iLumaTable=NULL;
	delete iChromaTable;
	iChromaTable=NULL;
	}

void CScaledJpegGenerator::CleanResultData()
	{
	iJPegImage.ResetAndDestroy();
	}
	
void CScaledJpegGenerator::CleanData()
	{
	CleanTempData();
	CleanResultData();
	
	delete iImageDecoder;
	iImageDecoder=NULL;
	delete iBitmap;
	iBitmap=NULL;
	}

void CScaledJpegGenerator::GetScaledImageSize(TSize &aSize)
	{
	if (iBitmap)
		{
		aSize = iBitmap->SizeInPixels();
		}
	else
		{
		aSize = iSize;
		}
	}
