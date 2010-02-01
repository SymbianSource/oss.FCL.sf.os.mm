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
// BitmapConvwerterBody.cpp
// 
//

#include "BitmapTransformsBody.h"
#include "BitmapTransformsMain.h"
#include <icl/imageprocessor.h>
#include "ImageProcessorPriv.h"

const TInt KPixelBufferSize = 128;
const TInt KLinesPerCall = 10;

/*
*
* CBody
*
*/
CBitmapConverter::CBody::CBody()
: CActive(EPriorityLow), iPixelDes(0,0,0)
	{
	}

/*
*
* RunL
*
*/
void CBitmapConverter::CBody::RunL()
	{
	DoConvert();
	}

/*
*
* DoCancel
*
*/
void CBitmapConverter::CBody::DoCancel()
	{
	RequestComplete(KErrCancel);
	}

/*
*
* ConstructL
*
*/
void CBitmapConverter::CBody::ConstructL()
	{
	CActiveScheduler::Add(this);
	iImageProc = CErrorDiffuser::NewL();
	iPixelBuffer = new (ELeave) TRgb[KPixelBufferSize];
	TInt length = KPixelBufferSize * sizeof(TRgb);
	TUint8* ptr = reinterpret_cast<TUint8*>(iPixelBuffer);
	iPixelDes.Set( ptr, length, length );
	}

/*
*
* NewL Creates a CbitmapConverter
*
*/
CBitmapConverter::CBody* CBitmapConverter::CBody::NewL()
	{
	CBody* self = new (ELeave) CBody();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/*
*
* DoConvert Converts KLinesPerCall lines, then either queues a RunL
* or, if the conversion has been completed, completes the client's message
*
*/
void CBitmapConverter::CBody::DoConvert()
	{
	TInt linesLeftThisCall = KLinesPerCall;
	
	while ((linesLeftThisCall-- > 0) && (iPos.iY < iSrcSize.iHeight))
		{
		iImageProc->SetPos(iPos);
		for (iPos.iX = 0; iPos.iX < iSrcSize.iWidth; iPos.iX += KPixelBufferSize)
			{
			const TInt width = Min(KPixelBufferSize,iSrcSize.iWidth - iPos.iX);
			iSrcBitmap->GetScanLine(iPixelDes,iPos,width,ERgb);
			iImageProc->SetPixels(iPixelBuffer,width);
			}
		iPos.iY++;
		}
	
	if (iPos.iY == iSrcSize.iHeight)
		{
		iImageProc->FlushPixels();
		RequestComplete(KErrNone);
		}
	else
		{
		SelfComplete(KErrNone);
		}
	}

/*
*
* SelfComplete
* Queues a RunL
*
* @param aError
*
*/
void CBitmapConverter::CBody::SelfComplete(TInt aError)
	{
	TRequestStatus *status = &iStatus;
	SetActive();
	User::RequestComplete(status, aError);
	}

/*
*
* RequestComplete
* Completes the client's message
*
* @param aReason
*
*/
void CBitmapConverter::CBody::RequestComplete(TInt aReason)
	{
	ASSERT(iCopyStatus);
	TRequestStatus* status = iCopyStatus;
	User::RequestComplete(status, aReason);
	}

/*
*
* ~CBody
*
*/
CBitmapConverter::CBody::~CBody()
	{
	Cancel();
	delete [] iPixelBuffer;
	delete iImageProc;
	}

/*
*
* Convert 
* Converts one bitmap to another
*
* @param aStatus
* @param aDstBitmap
* @param aSrcBitmap
* @pre aStatus != 0
* @pre aDstBitmap handle != 0
* @pre aSrcBitmap handle != 0
* @pre aDstBitmap size != 0 on any dimension
* @pre aSrcBitmap size != 0 on any dimension
* @pre src bitmap size == destination bitmap size
* Should this method be promoted to a public interface 
* it is recommended to change some of the assertions
* used in preconditions to User::Leave with an error code
*
*/
void CBitmapConverter::CBody::Convert(TRequestStatus* aStatus, CFbsBitmap& aDstBitmap,CFbsBitmap& aSrcBitmap)
	{
	//[preconditon aStatus != 0]
	ASSERT( aStatus );
	//[ precondition bitmaps have been created ]
	ASSERT( aDstBitmap.Handle() != 0 );
	ASSERT( aSrcBitmap.Handle() != 0 );

	iCopyStatus = aStatus;
	*iCopyStatus = KRequestPending;
	if(aDstBitmap.ExtendedBitmapType()!=KNullUid || aSrcBitmap.ExtendedBitmapType()!=KNullUid)
        {
        RequestComplete(KErrNotSupported);
        return;
        }
	
	TSize srcSize;
	      srcSize = aSrcBitmap.SizeInPixels();
	TSize dstSize;
	      dstSize = aDstBitmap.SizeInPixels();
	//[ precondition no dimension is zero]
	ASSERT( srcSize.iWidth != 0 );
	ASSERT( srcSize.iHeight != 0 );
	ASSERT( dstSize.iWidth != 0 );
	ASSERT( dstSize.iHeight != 0 );
	//[ precondition bitmaps are of the same dimension]
	ASSERT( dstSize.iWidth == srcSize.iWidth );
	ASSERT( dstSize.iHeight == srcSize.iHeight );
	    
	iSrcBitmap = &aSrcBitmap;
	iSrcSize = aSrcBitmap.SizeInPixels();
	iPos.SetXY(0,0);
	TInt errCode = KErrNone;
	TRAP( errCode, iImageProc->PrepareL(aDstBitmap,iSrcSize));
	if( errCode != KErrNone )
		{
		RequestComplete(errCode);
		return;
		}
	
	SelfComplete(KErrNone);
	}
