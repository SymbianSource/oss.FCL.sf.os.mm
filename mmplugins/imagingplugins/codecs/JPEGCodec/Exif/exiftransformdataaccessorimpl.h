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

#ifndef EXIFTRANSFORMDATAACCESSORIMPL_H
#define EXIFTRANSFORMDATAACCESSORIMPL_H

/**
   @file
   @internalComponent
*/

#include "exiftransformdataaccessor.h"
#include "ExifGeneralConsts.h"
class CExifTransform;

	
/**
	Class representing a file from where ExifEditUtility
	should read its input.
*/  
class CExifFileSource : public CBase, public MExifSource
	{
public:
	CExifFileSource(const TFileName& aFileName, RFs& aFs);
	~CExifFileSource();
	void InitL();
	TInt SetReadPosition(TInt aPosition);
	TUint16 ReadUint16L() const;
	CExifTransform* ReadAndConvertExifDataL(TUint aPos, TUint aLength) const;
	void ReadL(TUint aPos,TBufPtr8& aDes, TInt aLength, TRequestStatus& aStatus) const;
	void NextReadL(TBufPtr8& aDes, TInt aLength, TRequestStatus& aStatus) const;
	TInt Size(TInt& aSize) const;	
	CImageDecoder* CreateImageDecoderL(CImageDecoder::TOptions aDecoderOptions = CImageDecoder::EOptionNone) const;
	TInt Cancel();
	void CleanupAfterEarlyError();
	void CleanupAfterLateError();
	void Destroy();
private:
	void DeleteDataBuffer() const;
	void PrepareDataBufferL(TBufPtr8& aDes, const TInt aLength) const;
private:
	enum 
		{
		EMinimumBufferSize = KIOBlockSize
		};
	TFileName iFileName;
	RFile iFileHandle;
	RFs& iFs;
	mutable HBufC8* iDataBuffer; // Owned. Holds the block of data read in a ReadL pass
	mutable TBuf8<sizeof(TUint16)> i16BitValueStr; // Scratch variable used by ReadUint16L
	};
	

	
/**	
	Class representing a descriptor from where ExifEditUtility
	should read its input.
*/    
class CExifDescSource : public CBase, public MExifSource
	{
public:
	CExifDescSource(const TDesC8& aSource, RFs& aFs);
	~CExifDescSource();
	void InitL();
	TInt SetReadPosition(TInt aPosition);
	TUint16 ReadUint16L() const;
	CExifTransform* ReadAndConvertExifDataL(TUint aPos, TUint aLength) const;
	void ReadL(TUint aPos,TBufPtr8& aDes, TInt aLength, TRequestStatus& aStatus) const;
	void NextReadL(TBufPtr8& aDes, TInt aLength,TRequestStatus& aStatus) const;
	TInt Size(TInt& aSize) const;	
	CImageDecoder* CreateImageDecoderL(CImageDecoder::TOptions aDecoderOptions = CImageDecoder::EOptionNone) const;
	TInt Cancel();
	void CleanupAfterEarlyError();
	void CleanupAfterLateError();
	void Destroy();
private:
	const TDesC8* iSourceDes;		// The source descriptor that will be read (not owned)
	mutable TUint iSourceDesPos;	// The current postion in the source descriptor   	
	RFs& iFs;
	};
	
	
/**
	Class representing a file to which ExifEditUtility
	should write its output.
*/ 	
class CExifFileDest : public CBase, public MExifDest
	{
public:
	CExifFileDest(const TFileName& aFileName, RFs& aFs);
	~CExifFileDest();
	TInt Init();
	void WriteL(const TDesC8& aDes, TInt aLength, TRequestStatus& aStatus);
	void WriteDestBufferL(TRequestStatus& aStatus);
	void SetDestBuffer(HBufC8* aBuffer);
	void WriteComplete();
	TInt Cancel();
	void CleanupAfterEarlyError();
	void CleanupAfterLateError();
	void Destroy();
private:
	void DeleteDestBuffer();
private:
	TFileName iFileName;
	RFile iFileHandle;
	RFs& iFs;
	HBufC8* iDestBuffer; //Buffer used to hold data that needs to be owned until it has been written to the destination
	};


/**
	Class representing a descriptor to which ExifEditUtility
	should write its output.
*/	
class CExifDescDest : public CBase, public MExifDest
	{
public:
	CExifDescDest(HBufC8*& aDest);
	~CExifDescDest();
	TInt Init();
	void WriteL(const TDesC8& aDes, TInt aLength, TRequestStatus& aStatus);
	void WriteDestBufferL(TRequestStatus& aStatus);
	void SetDestBuffer(HBufC8* aBuffer);
	void WriteComplete();
	TInt Cancel();
	void CleanupAfterEarlyError();
	void CleanupAfterLateError();
	void Destroy();
private:
	void DeleteDestBuffer();
private:
	HBufC8* iDestDes; 		// The destination descriptor to write the image to (owned until passed to client)
	HBufC8** iClientDestDes;// The client pointer to the destination descriptor (not owned)
	TUint iDestDesPos; 		// The write position of the destination descriptor
	HBufC8* iDestBuffer;	// Buffer used to hold data that needs to be owned until it has been written to the destination
	};
	
#endif //EXIFTRANSFORMDATAACCESSORIMPL_H
