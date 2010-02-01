// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "JpegYuvDecoder.h"

/**
Start decoding an image frame asynchronously. Pixels will be decoded in
YUV format.

@pre
The destination bitmap that receives the decoded data must be created before 
using this function. The bitmap must be large enough to hold the frame and the display
mode must be set to EColor16M. Use CImageDecoder::FrameInfo() to determine the
size of the frame.

@post
On completion, aRequestStatus contains an error code. If the frame was decoded successfully it
will contain KErrNone. If frame was only partially decoded it will contain KErrUnderflow
or another of the system-wide error codes.

@param  aRequestStatus
        The request status.
@param  aDestination
        A bitmap that will contain the decoded frame.
@param  aFrameNumber
        The frame in multi-frame image to decode (optional, in which case defaults to zero).
*/
EXPORT_C void CJpegYuvDecoder::ConvertYuv(TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, TInt aFrameNumber)
	{
	ASSERT(aDestination.DisplayMode() == EColor16M);
	
	if(aDestination.Handle()!=0 && aDestination.ExtendedBitmapType()!=KNullUid)
	    {
	    TRequestStatus* status = aRequestStatus;
	    User::RequestComplete(status,KErrNotSupported);
	    }

	TRAPD(error,CustomSyncL(EOptionYuvDecode));
	if (error!=KErrNone)
		{
		TRequestStatus* status = aRequestStatus;
		User::RequestComplete(status, error);
		}

	CImageDecoder::Convert(aRequestStatus, aDestination, aFrameNumber);
	}

/**
Start decoding an image frame and mask asynchronously. Pixels will be decoded in
YUV format.

@pre
The destination bitmap, aDestination must be created before the call and must be large enough
for the frame and the display mode must be EColor16M. FrameInfo() can be used to obtain the
size and display mode of the frame. The mask bitmap, aDestinationMask must also be created before
the call and must be large enough for the mask. The display mode must be EGray2 or EGray256
and must be EGray256 if the image contains alpha-blending information. This information can 
be obtained from the iFlags property of TFrameInfo obtained from a FrameInfo() call.

@param  aRequestStatus
        The request status. On completion contains an error code.
        KErrNone if frame was decoded successfully,
        KErrUnderflow if the frame was partially decoded
        otherwise another of the system-wide error codes.
@param  aDestination
        A bitmap that will contain the decoded frame.
@param  aDestinationMask
        A bitmap that will contain the decoded frame mask.
@param  aFrameNumber
        The frame in multi-frame image to decode (optional, in which case defaults to zero).

@see    TFrameInfo
*/
EXPORT_C void CJpegYuvDecoder::ConvertYuv(TRequestStatus* aRequestStatus, CFbsBitmap& aDestination, CFbsBitmap& aDestinationMask, TInt aFrameNumber)
	{
	ASSERT(aDestination.DisplayMode() == EColor16M);
	
	if(aDestination.Handle()!=0 && aDestination.ExtendedBitmapType()!=KNullUid)
	        {
	        TRequestStatus* status = aRequestStatus;
	        User::RequestComplete(status,KErrNotSupported);
	        }
	
	if(aDestinationMask.Handle()!=0 && aDestinationMask.ExtendedBitmapType()!=KNullUid)
	        {
	        TRequestStatus* status = aRequestStatus;
	        User::RequestComplete(status,KErrNotSupported);
	        }

	TRAPD(error, CustomSyncL(EOptionYuvDecode));
	if (error!=KErrNone)
		{
		TRequestStatus* status = aRequestStatus;
		User::RequestComplete(status, error);
		}

	CImageDecoder::Convert(aRequestStatus, aDestination, aDestinationMask, aFrameNumber);
	}

