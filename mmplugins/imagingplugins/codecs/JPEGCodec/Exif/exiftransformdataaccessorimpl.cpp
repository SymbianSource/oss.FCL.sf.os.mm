// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @internalComponent
*/

#include "exiftransformdataaccessorimpl.h"
#include "exiftransform.h"
#include "ImageUtils.h"


//
//
// CExifFileSource
//
//
//

CExifFileSource::CExifFileSource(const TFileName& aFileName, RFs& aFs)
: iFileName(aFileName), iFs(aFs)
	{	
	i16BitValueStr.SetLength(sizeof(TUint16));
	}
	
CExifFileSource::~CExifFileSource()
	{
	DeleteDataBuffer();
	iFileHandle.Close();	
	}
	
/**
Opens the source file
*/
void CExifFileSource::InitL()
	{
	TInt err = iFileHandle.Open(iFs, iFileName, EFileShareReadersOnly);
	if(err != KErrNone && err != KErrNoMemory)
		{
		err = iFileHandle.Open(iFs, iFileName, EFileShareAny);
		}
	User::LeaveIfError(err);
	}
	
TInt CExifFileSource::SetReadPosition(TInt aPosition)
	{
	return iFileHandle.Seek(ESeekStart, aPosition);	
	}
	
/**
Reads & returns a TUint16 from the current file position
*/
TUint16 CExifFileSource::ReadUint16L() const
	{		
	User::LeaveIfError(iFileHandle.Read(i16BitValueStr, 2));
	return PtrReadUtil::ReadBigEndianUint16(&i16BitValueStr[0]);
	}

/**
Reads the Exif metadata block of the specified length from the source, 
starting at the specified position and converts it into a new CExifData
object to be owned by the caller.  It is expected that aPos is correct
for non-zero lengths.		
*/
CExifTransform* CExifFileSource::ReadAndConvertExifDataL(const TUint aPos, const TUint aLength) const
	{
	if(aLength)
		{
		TBufPtr8 ptrToExifData;
		PrepareDataBufferL(ptrToExifData, aLength);
		User::LeaveIfError(iFileHandle.Read(aPos, ptrToExifData, aLength));
		CExifTransform* exifdata = CExifTransform::NewL(ptrToExifData.PtrZ(), ptrToExifData.Length());
		DeleteDataBuffer();
		return exifdata;
		}
	else
		{
		return CExifTransform::NewL(NULL);
		}
	}

/**
Reads aLength bytes of data from the source, starting at position aPos, into the
the object's HBufC8. Updates aDes to point to this HBufC8.  Ownership
of the HBufC8 is not transferred.
*/
void CExifFileSource::ReadL(const TUint aPos, TBufPtr8& aDes, const TInt aLength, TRequestStatus& aStatus) const
	{
	PrepareDataBufferL(aDes, aLength);
	iFileHandle.Read(aPos, aDes, aLength, aStatus);
	}
	
/**
Reads aLength bytes of data from the source into the HBufC8 to which aDes currently points.
The source is read from its current position. Assumes that the input aDes has 
previously been set-up to point to an HBufC8 in an earlier call to ReadL.
*/
void CExifFileSource::NextReadL(TBufPtr8& aDes,TInt aLength,TRequestStatus& aStatus) const
	{
	PrepareDataBufferL(aDes, aLength);
	iFileHandle.Read(aDes, aLength, aStatus);
	}
	
/**
Returns the size of the source data
*/
TInt CExifFileSource::Size(TInt& aSize) const
	{
	return iFileHandle.Size(aSize);	
	}
	
/**
Creates a new CImageDecoder object for files and returns it to
the caller.  It will be owned by the caller.
*/
CImageDecoder* CExifFileSource::CreateImageDecoderL(CImageDecoder::TOptions aDecoderOptions) const
	{
	return CImageDecoder::FileNewL(iFs, iFileName, aDecoderOptions, KNullUid);	
	}	
	
/**
Allocates internal buffer of length aLength and updates aDes to point to it.
*/
void CExifFileSource::PrepareDataBufferL(TBufPtr8& aDes, const TInt aLength) const
	{
	if (iDataBuffer==NULL)
		{
		TInt len = aLength < EMinimumBufferSize ? EMinimumBufferSize : aLength;
		iDataBuffer=HBufC8::NewL(len+1); // +1 needed to ensure we have the null trailer	
		}
	else if (aLength+1 > iDataBuffer->Des().MaxLength())
		{
		iDataBuffer = iDataBuffer->ReAllocL(aLength+1);
		}
	aDes.Set(iDataBuffer->Des());
	}
		
void CExifFileSource::DeleteDataBuffer() const
	{
	delete iDataBuffer;
	iDataBuffer=NULL;
	}

/**
Called when transform is cancelled	
*/
TInt CExifFileSource::Cancel()
	{
	return KErrNone;
	}

/**
Called from ExifEditUtility::RunError before EReadComplete state reached	
*/
void CExifFileSource::CleanupAfterEarlyError()
	{
	}

/**
Called from ExifEditUtility::RunError after EReadComplete state reached	
*/
void CExifFileSource::CleanupAfterLateError()
	{
	}
	
/**
Called to destroy the object when one only has a pointer to the abstract class.
*/
void CExifFileSource::Destroy()
	{
	delete this;
	}

//
//
// CExifDescSource
//
//
//

CExifDescSource::CExifDescSource(const TDesC8& aSource, RFs& aFs)
: iSourceDes(&aSource), iFs(aFs)
	{	
	}
	
CExifDescSource::~CExifDescSource()
	{	
	}
	

void CExifDescSource::InitL()
	{
	iSourceDesPos = 0;
	}

TInt CExifDescSource::SetReadPosition(TInt aPosition)
	{
	if ((aPosition >= 0) && (aPosition < iSourceDes->Size()))
		{
		iSourceDesPos = aPosition;
		return KErrNone;	
		}
	return KErrArgument;
	}
	
/**
Reads & returns a TUint16 from the current position. Updates current position.
*/
TUint16 CExifDescSource::ReadUint16L() const
	{		
	const TUint8* sourceDataPtr = iSourceDes->Ptr() + iSourceDesPos;
	TUint16 result = PtrReadUtil::ReadBigEndianUint16(sourceDataPtr);
	iSourceDesPos+=sizeof(TUint16);
	return result;
	}

/**
Reads the Exif metadata block of the specified length from the source, 
starting at the specified position and converts it into a new CExifData
object to be owned by the caller.  It is expected that aPos is correct
for non-zero lengths.		
*/
CExifTransform* CExifDescSource::ReadAndConvertExifDataL(const TUint aPos, const TUint aLength) const
	{
	if (aLength)
		{		
		return CExifTransform::NewL(iSourceDes->Ptr() + aPos, aLength);
		}
	else
		{
		return CExifTransform::NewL(NULL);
		}
	}

/**
Reads aLength bytes of data from the source, starting at position aPos, into the
the object's HBufC8. Updates aDes to point to this HBufC8.  Ownership
of the HBufC8 is not transferred.
*/
void CExifDescSource::ReadL(const TUint aPos, TBufPtr8& aDes, const TInt aLength, TRequestStatus& aStatus) const
	{	
	TPtrC8 sourceBlock(iSourceDes->Ptr() + aPos, aLength);
	aDes.Set(sourceBlock);
	iSourceDesPos = aPos+aLength;
	aStatus=KRequestPending;
	TRequestStatus* reqStat=&aStatus;
	User::RequestComplete(reqStat, KErrNone);
	}
	
/**
Reads aLength bytes of data from the source into the HBufC8 to which aDes currently points.
The source is read from its current position. Assumes that the input aDes has 
previously been set-up to point to an HBufC8 in an earlier call to ReadL.
*/
void CExifDescSource::NextReadL(TBufPtr8& aDes, TInt aLength, TRequestStatus& aStatus) const
	{
	ReadL(iSourceDesPos, aDes, aLength, aStatus);
	}
	
/**
Returns the size of the source data
*/
TInt CExifDescSource::Size(TInt& aSize) const
	{
	aSize = iSourceDes->Length();
	return KErrNone;	
	}
	
/**
Creates a new CImageDecoder object for data and returns it to
the caller.  It will be owned by the caller.
*/
CImageDecoder* CExifDescSource::CreateImageDecoderL(CImageDecoder::TOptions aDecoderOptions) const
	{
	return CImageDecoder::DataNewL(iFs, *iSourceDes, aDecoderOptions, KNullUid);
	}		

/**
Called when transform is cancelled
*/
TInt CExifDescSource::Cancel()
	{
	iSourceDesPos = 0; //reset source descriptor read position
	return KErrNone;	
	}
	
/**
Called from ExifEditUtility::RunError before EReadComplete state reached
*/
void CExifDescSource::CleanupAfterEarlyError()
	{
	iSourceDesPos = 0; //reset source descriptor read position	
	}

/**
Called from ExifEditUtility::RunError after EReadComplete state reached	
*/
void CExifDescSource::CleanupAfterLateError()
	{
	iSourceDesPos = 0; //reset source descriptor read position	
	}

/**
Called to destroy the object when one only has a pointer to the abstract class.
*/
void CExifDescSource::Destroy()
	{
	delete this;
	}
	
//
//
// CExifFileDest (file destination)
//
//
//

CExifFileDest::CExifFileDest(const TFileName& aFileName, RFs& aFs)
: iFileName(aFileName), iFs(aFs)
	{
	}
	
CExifFileDest::~CExifFileDest()
	{
	DeleteDestBuffer();
	iFileHandle.Close();	
	}
	
TInt CExifFileDest::Init()
	{
	if(iFileHandle.Open(iFs, iFileName, EFileWrite)==KErrNone)
		{
		iFileHandle.Close();
		iFs.Delete(iFileName);
		}
	return iFileHandle.Create(iFs, iFileName, EFileWrite);
	}

void CExifFileDest::WriteComplete()
	{
	iFileHandle.Close();
	}

/**
Writes aLength bytes of aDes to the destination file & updates aStatus 	
*/
void CExifFileDest::WriteL(const TDesC8& aDes,TInt aLength,TRequestStatus& aStatus)
	{
	iFileHandle.Write(aDes, aLength, aStatus);	
	}

/**
Writes the internal buffer owned by the object to the destination file
*/
void CExifFileDest::WriteDestBufferL(TRequestStatus& aStatus)
	{
	if (iDestBuffer)
		{
		iFileHandle.Write(*iDestBuffer, aStatus);	
		}
	else
		{
		aStatus=KRequestPending;
		TRequestStatus* reqStat=&aStatus;
		User::RequestComplete(reqStat, KErrNone);			
		}
	}
		
/**
Called when transform is cancelled	
*/
TInt CExifFileDest::Cancel()
	{
	iFileHandle.Close();
	return iFs.Delete(iFileName);
	}

/**
Called from ExifEditUtility::RunError before EReadComplete state reached	
*/
void CExifFileDest::CleanupAfterEarlyError()
	{
	}

/**
Called from ExifEditUtility::RunError after EReadComplete state reached	
*/
void CExifFileDest::CleanupAfterLateError()
	{
	Cancel();
	}	

/**
Called to destroy the object when one only has a pointer to the abstract class.
*/
void CExifFileDest::Destroy()
	{
	delete this;
	}
		
/**
Transfers ownership of aBuffer to the object
*/
void CExifFileDest::SetDestBuffer(HBufC8* aBuffer)
	{
	DeleteDestBuffer();
	iDestBuffer = aBuffer;
	}

void CExifFileDest::DeleteDestBuffer()
	{
	delete iDestBuffer;
	iDestBuffer=NULL;
	}
	
//
//
// CExifDescDest (descriptor destination)
//
//
//


/**
Client supplies (NULL) HBufC8* which will be set to point to an
HBufC8 containing the output of ExifEditUtility. This buffer will
be owned by the client.
*/
CExifDescDest::CExifDescDest(HBufC8*& aDest)
	{
	iClientDestDes = &aDest;
	}
	
CExifDescDest::~CExifDescDest()
	{
	DeleteDestBuffer();
	//only delete iDestDes if ownership has not been transferred to the client
	if (iClientDestDes && iDestDes)
		{
		if (*iClientDestDes == NULL)
			{
			//ownership has not yet been transferred to the client so delete
			delete iDestDes;
			}
		}
	}
	
TInt CExifDescDest::Init()
	{
	return KErrNone;
	}
	
/**
WriteComplete is called by ExifEditUtility after all data has been written to 
the destination. It sets the client's pointer to point to this destination.
Client owns the data after this point.
*/
void CExifDescDest::WriteComplete()
	{
	*iClientDestDes = iDestDes;
	iDestDes = NULL;
	}

/**
Writes aLength bytes of aDes to the destination descriptor & updates aStatus	
*/
void CExifDescDest::WriteL(const TDesC8& aDes,TInt aLength,TRequestStatus& aStatus)
	{
	if (iDestDes)
		{
		TInt destDesLength = iDestDes->Length();
		iDestDes = iDestDes->ReAllocL(destDesLength+aLength);
		}
	else
		{
		iDestDes = HBufC8::NewL(aLength);
		}
		
	TPtr8 destDesPtr(const_cast<TUint8*>(iDestDes->Ptr())+iDestDesPos, aLength);
	destDesPtr.Copy(aDes);
	iDestDesPos += aLength;
	iDestDes->Des().SetLength(iDestDesPos);	
	aStatus=KRequestPending;
	TRequestStatus* reqStat=&aStatus;
	User::RequestComplete(reqStat, KErrNone);	
	}

/**
Writes the internal buffer owned by the object to the destination descriptor
*/
void CExifDescDest::WriteDestBufferL(TRequestStatus& aStatus)
	{
	if (iDestBuffer)
		{
		WriteL(*iDestBuffer, iDestBuffer->Length(), aStatus);	
		}
	else
		{
		aStatus=KRequestPending;
		TRequestStatus* reqStat=&aStatus;
		User::RequestComplete(reqStat, KErrNone);			
		}
	}

/**
Called when transform is cancelled
*/
TInt CExifDescDest::Cancel()
	{
	delete iDestDes;
	iDestDes = NULL;
	iDestDesPos = 0;
	return KErrNone;
	}

/**
Called from ExifEditUtility::RunError before EReadComplete state reached	
*/
void CExifDescDest::CleanupAfterEarlyError()
	{
	Cancel();
	}

/**
Called from ExifEditUtility::RunError after EReadComplete state reached	
*/
void CExifDescDest::CleanupAfterLateError()
	{
	}

/**
Called to destroy the object when one only has a pointer to the abstract class.
*/
void CExifDescDest::Destroy()
	{
	delete this;
	}
	
/**
Transfers ownership of aBuffer to the object
*/
void CExifDescDest::SetDestBuffer(HBufC8* aBuffer)
	{
	DeleteDestBuffer();
	iDestBuffer = aBuffer;
	}

void CExifDescDest::DeleteDestBuffer()
	{
	delete iDestBuffer;
	iDestBuffer=NULL;
	}
