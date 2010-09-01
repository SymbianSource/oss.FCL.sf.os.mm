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

#include <e32property.h>
#include "fwexttestcodec.h"

// Read Codec
CFwExtReadCodec* CFwExtReadCodec::NewL(CFwExtTestDecoderPlugin& aDecoder)
	{
	return new (ELeave) CFwExtReadCodec(aDecoder);
	}

CFwExtReadCodec::CFwExtReadCodec(CFwExtTestDecoderPlugin& aDecoder)
:iDecoder(&aDecoder)
	{
	}

void CFwExtReadCodec::InitFrameL(TFrameInfo& /*aFrameInfo*/, CFrameImageData& /*aFrameImageData*/, TBool /*aDisableErrorDiffusion*/, CFbsBitmap& /*aDestination*/, CFbsBitmap* /*aDestinationMask*/)
	{
	}
	
void CFwExtReadCodec::InitFrameHeader(TFrameInfo& aFrameInfo, CFrameImageData& /*aFrameData*/)
	{
	aFrameInfo.SetCurrentFrameState(TFrameInfo::EFrameInfoProcessingComplete);
	}
	
void CFwExtReadCodec::SetPrepareProcessing()
	{
	iPrepareProcessing = ETrue;
	}
	
TFrameState CFwExtReadCodec::ProcessFrameL(TBufPtr8& /*aSrc*/)
	{
	if ( iPrepareProcessing )
		{
		// Prepare called. We ask Framework to call us four times before completing
		if(iLoops == 3)
			{
			iPrepareProcessing = EFalse;
			return EFrameComplete; // prepare processed successfully
			}
		iLoops++;
		return EFrameIncomplete; // processing of block is incomplete
		}
	else
		{
		// Convert called
		// Would process data passed from the extension
		return EFrameComplete;
		}
	}

// Write Codec
CFwExtWriteCodec* CFwExtWriteCodec::NewL(CFwExtTestEncoderPlugin& aEncoder)
	{
	return new (ELeave) CFwExtWriteCodec(aEncoder);
	}

CFwExtWriteCodec::CFwExtWriteCodec(CFwExtTestEncoderPlugin& aEncoder)
:iEncoder(&aEncoder)
	{
	}

void CFwExtWriteCodec::ResetCount()
	{
	iLoops = 0;
	}
	
void CFwExtWriteCodec::Complete()
	{
	iComplete = ETrue;
	}

void CFwExtWriteCodec::SetPrepareProcessing()
	{
	iPrepareProcessing = ETrue;
	}

TFrameState CFwExtWriteCodec::ProcessFrameL(TBufPtr8& /*aDst*/)
	{
	if ( iPrepareProcessing )
		{
		iPrepareProcessing = EFalse;
		return EFrameComplete; // prepare processed successfully
		}
	else if(iComplete)
		{
		return EFrameComplete; // to indicate completion of image.
		}
	else
		{
		if(iLoops == 5)
			{
			return EBlockComplete; // block processed successfully
			}
		iLoops++;
		return EFrameIncomplete; // processing of block is incomplete
		}
	}
