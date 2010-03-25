/*
* Copyright (c) 2010 ISB.
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the "Symbian Foundation License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.symbianfoundation.org/legal/sfl-v10.html".
*
* Initial Contributors:
* ISB - Initial contribution
*
* Contributors:
*
* Description:
*
*/

#include <fbs.h>
#include "ecamwebcamerabuffer.h"


// ---------------------------------------------------------------------------
// NewL
// Takes ownership of aData, but not aBitmap
// ---------------------------------------------------------------------------
//
CWebCameraBuffer* CWebCameraBuffer::NewL(const CFbsBitmap& aBitmap, HBufC8* aData)
	{
	CWebCameraBuffer* self = CWebCameraBuffer::NewLC( aBitmap, aData );
	CleanupStack::Pop( self );
	return self;
	}

// ---------------------------------------------------------------------------
// NewLC
// Takes ownership of aData, but not aBitmap
// ---------------------------------------------------------------------------
//
CWebCameraBuffer* CWebCameraBuffer::NewLC(const CFbsBitmap& aBitmap, HBufC8* aData)
	{
	CWebCameraBuffer* self = new (ELeave) CWebCameraBuffer;
	CleanupStack::PushL( self );
	self->ConstructL( aBitmap, aData );
	return self;
	}


// ---------------------------------------------------------------------------
// NewL
// Takes ownership of both objects.
// ---------------------------------------------------------------------------
//
CWebCameraBuffer* CWebCameraBuffer::NewL(CFbsBitmap* aBitmap, HBufC8* aData)
	{
	CWebCameraBuffer* self = CWebCameraBuffer::NewLC( aBitmap, aData );
	CleanupStack::Pop( self );
	return self;
	}

// ---------------------------------------------------------------------------
// NewLC
// Takes ownership of both objects.
// ---------------------------------------------------------------------------
//
CWebCameraBuffer* CWebCameraBuffer::NewLC(CFbsBitmap* aBitmap, HBufC8* aData)
	{
	CWebCameraBuffer* self = new (ELeave) CWebCameraBuffer;
	CleanupStack::PushL( self );
	self->ConstructL( aBitmap, aData );
	return self;
	}

// ---------------------------------------------------------------------------
// Constructor
//
// ---------------------------------------------------------------------------
//
CWebCameraBuffer::CWebCameraBuffer()
	: iBitmap(NULL),
	  iImageData(NULL),
	  iOwnBitmap(ETrue),
	  iOwnData(ETrue)
	{
	}

// ---------------------------------------------------------------------------
// Destructor
// *private, because Release() is supposed to be used.*
// ---------------------------------------------------------------------------
//
CWebCameraBuffer::~CWebCameraBuffer()
	{
	iChunk.Close();
	delete iBitmap;
	delete iImageData;
	}

// ---------------------------------------------------------------------------
// ConstructL
//
// ---------------------------------------------------------------------------
//
void CWebCameraBuffer::ConstructL(const CFbsBitmap& aBitmap, HBufC8* aData)
	{
	iBitmap = new (ELeave) CFbsBitmap;
	TInt error = iBitmap->Duplicate( aBitmap.Handle() );

	iImageData = aData;

	User::LeaveIfError( error );
	}

// ---------------------------------------------------------------------------
// ConstructL
//
// ---------------------------------------------------------------------------
//
void CWebCameraBuffer::ConstructL(CFbsBitmap* aBitmap, HBufC8* aData)
	{
	iImageData = aData;
	iBitmap    = aBitmap;
	}

// ---------------------------------------------------------------------------
// NumFrames
//
// Number of frames available in the buffer.
// ---------------------------------------------------------------------------
//
TInt 
CWebCameraBuffer::NumFrames()
	{
	return 1;
	}


// ---------------------------------------------------------------------------
// DataL
//
// Frame data as descriptor.
// *not supported here*
// ---------------------------------------------------------------------------
//
TDesC8* CWebCameraBuffer::DataL(TInt aFrameIndex)
	{
	if( aFrameIndex != 0 || !iImageData )
		{
		User::Leave( KErrNotSupported );
		}

	return iImageData; 
	}

// ---------------------------------------------------------------------------
// BitmapL
//
// Frame data as bitmap.
// ---------------------------------------------------------------------------
//
CFbsBitmap& CWebCameraBuffer::BitmapL(TInt aFrameIndex)
	{
	if( aFrameIndex != 0 || !iBitmap )
		{
		User::Leave( KErrNotFound );
		}

	return *iBitmap;
	}

// ---------------------------------------------------------------------------
// DataL
//
// Frame data as chunk.
// *not supported here*
// ---------------------------------------------------------------------------
//
RChunk& CWebCameraBuffer::ChunkL()
	{
	User::Leave( KErrNotSupported );

	return iChunk;
	}

// ---------------------------------------------------------------------------
// ChunkOffsetL
//
// Frame data offset in chunk.
// *not supported here*
// ---------------------------------------------------------------------------
//
TInt 
CWebCameraBuffer::ChunkOffsetL(TInt /*aFrameIndex*/)
	{
	User::Leave( KErrNotSupported );

	return 0;
	}

// ---------------------------------------------------------------------------
// FrameSize
//
// Frame data size.
// *not supported here, as only bitmap supported*
// ---------------------------------------------------------------------------
//
TInt CWebCameraBuffer::FrameSize(TInt /*aFrameIndex*/)
	{
	return -1;
	}

// ---------------------------------------------------------------------------
// Release
//
// Release this buffer.
// Simply deletes this object.
// ---------------------------------------------------------------------------
//
void CWebCameraBuffer::Release()
	{
	delete this;
	}


// end of file
